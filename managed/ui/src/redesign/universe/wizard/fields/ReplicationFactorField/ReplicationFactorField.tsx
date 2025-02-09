import React, { FC } from 'react';
import { Controller, useFormContext } from 'react-hook-form';
import { ReplicationFactor } from './ReplicationFactor';
import { CloudConfigFormValue } from '../../steps/cloud/CloudConfig';

interface ReplicationFactorFieldProps {
  disabled: boolean;
}

const REPLICATION_FACTORS = [1, 3, 5];

export const ReplicationFactorField: FC<ReplicationFactorFieldProps> = ({ disabled }) => {
  const { control } = useFormContext<CloudConfigFormValue>();

  return (
    <Controller
      control={control}
      name="replicationFactor"
      render={({ field: { value, onChange } }) => (
        <ReplicationFactor
          value={value}
          onChange={onChange}
          options={REPLICATION_FACTORS}
          disabled={disabled}
        />
      )}
    />
  );
};
