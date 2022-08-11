//
// Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//
//

#include "yb/util/metrics_writer.h"

#include <regex>

#include "yb/util/enums.h"

namespace yb {

PrometheusWriter::PrometheusWriter(std::stringstream* output)
    : output_(output),
      timestamp_(std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch()).count()) {}

PrometheusWriter::~PrometheusWriter() {}

Status PrometheusWriter::FlushAggregatedValues(
    uint32_t max_tables_metrics_breakdowns, const std::string& priority_regex) {
  uint32_t counter = 0;
  std::regex p_regex(priority_regex);
  for (const auto& [_, data] : tables_) {
    const auto& attrs = data.attributes;
    for (const auto& metric_entry : data.values) {
      if (priority_regex.empty() || std::regex_match(metric_entry.first, p_regex)) {
        RETURN_NOT_OK(FlushSingleEntry(attrs, metric_entry.first, metric_entry.second));
      }
    }
    if (++counter >= max_tables_metrics_breakdowns) {
      break;
    }
  }
  return Status::OK();
}

Status PrometheusWriter::FlushSingleEntry(
    const MetricEntity::AttributeMap& attr,
    const std::string& name, const int64_t value) {
  LOG(INFO) << "suresh: name: " << name << " value: " << value;
  *output_ << name;
  size_t total_elements = attr.size();
  if (total_elements > 0) {
    *output_ << "{";
    for (const auto& entry : attr) {
      *output_ << entry.first << "=\"" << entry.second << "\"";
      if (--total_elements > 0) {
        *output_ << ",";
      }
    }
    *output_ << "}";
  }
  *output_ << " " << value;
  *output_ << " " << timestamp_;
  *output_ << std::endl;
  return Status::OK();
}

void PrometheusWriter::InvalidAggregationFunction(AggregationFunction aggregation_function) {
  FATAL_INVALID_ENUM_VALUE(AggregationFunction, aggregation_function);
}

Status PrometheusWriter::WriteSingleEntry(
    const MetricEntity::AttributeMap& attr, const std::string& name, int64_t value,
    AggregationFunction aggregation_function) {
  auto it = attr.find("table_id");
  //LOG(INFO) << "suresh: name: " << name << " value: " << value;

  if (it == attr.end()) {
    //LOG(INFO) << "suresh: name: " << name << " value: " << value;
    return FlushSingleEntry(attr, name, value);
  }

  // For tablet level metrics, we roll up on the table level.
  auto table_it = tables_.find(it->second);
  if (table_it == tables_.end()) {
    // If it's the first time we see this table, create the aggregate structures.
    table_it = tables_.emplace(it->second, TableData { .attributes = attr, .values = {} }).first;
    table_it->second.values.emplace(name, value);
  } else {
    auto& stored_value = table_it->second.values[name];
    LOG(INFO) << "suresh: for name: " << name << "  stored_value: " << stored_value;

    switch (aggregation_function) {
      case kSum:
        stored_value += value;
        break;
      case kMax:
        // If we have a new max, also update the metadata so that it matches correctly.
        if (value > stored_value) {
          table_it->second.attributes = attr;
          stored_value = value;
        }
        break;
      default:
        InvalidAggregationFunction(aggregation_function);
        break;
    }
  }
  return Status::OK();
}

NMSWriter::NMSWriter(EntityMetricsMap* table_metrics, MetricsMap* server_metrics)
    : PrometheusWriter(nullptr), table_metrics_(*table_metrics),
      server_metrics_(*server_metrics) {}

Status NMSWriter::FlushSingleEntry(
    const MetricEntity::AttributeMap& attr, const std::string& name, const int64_t value) {
  auto it = attr.find("table_id");
  if (it != attr.end()) {
    table_metrics_[it->second][name] = value;
    return Status::OK();
  }
  it = attr.find("metric_type");
  if (it == attr.end() || it->second != "server") {
    return Status::OK();
  }
  server_metrics_[name] = value;
  return Status::OK();
}

} // namespace yb
