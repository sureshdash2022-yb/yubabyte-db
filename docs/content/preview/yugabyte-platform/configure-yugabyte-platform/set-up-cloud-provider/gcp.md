---
title: Configure the GCP cloud provider
headerTitle: Configure the GCP cloud provider
linkTitle: Configure cloud providers
description: Configure the Google Cloud Platform (GCP) cloud provider.
aliases:
  - /preview/deploy/enterprise-edition/configure-cloud-providers/gcp
menu:
  preview:
    identifier: set-up-cloud-provider-2-gcp
    parent: configure-yugabyte-platform
    weight: 20
isTocNested: false
showAsideToc: true
---

<ul class="nav nav-tabs-alt nav-tabs-yb">

  <li>
    <a href="../aws/" class="nav-link">
      <i class="fab fa-aws"></i>
      AWS
    </a>
  </li>

  <li>
    <a href="../gcp/" class="nav-link active">
      <i class="fab fa-google" aria-hidden="true"></i>
      GCP
    </a>
  </li>

  <li>
    <a href="../azure/" class="nav-link">
      <i class="icon-azure" aria-hidden="true"></i>
      &nbsp;&nbsp; Azure
    </a>
  </li>

  <li>
    <a href="../kubernetes/" class="nav-link">
      <i class="fas fa-cubes" aria-hidden="true"></i>
      Kubernetes
    </a>
  </li>

  <li>
    <a href="../vmware-tanzu/" class="nav-link">
      <i class="fas fa-cubes" aria-hidden="true"></i>
      VMware Tanzu
    </a>
  </li>

<li>
    <a href="../openshift/" class="nav-link">
      <i class="fas fa-cubes" aria-hidden="true"></i>OpenShift</a>
  </li>

  <li>
    <a href="../on-premises/" class="nav-link">
      <i class="fas fa-building"></i>
      On-premises
    </a>
  </li>

</ul>

You can configure Google Cloud Platform (GCP) for YugabyteDB clusters using YugabyteDB Anywhere. If no cloud providers are configured yet, the main Dashboard page prompts you to configure at least one cloud provider.

## Prerequisites

To run YugabyteDB nodes on GCP, all you need to provide on the YugabyteDB Anywhere UI is your cloud provider credentials. YugabyteDB Anywhere uses those credentials to automatically provision and de-provision instances that run YugabyteDB. An instance for YugabyteDB includes a compute instance, as well as local or remote disk storage attached to the compute instance.

## Configure GCP

You can configure GCP as follows:

- Navigate to **Configs > Google Cloud Platform**.

- Click **Add Configuration** to open the **Cloud Provider Configuration** page shown in the following illustration:<br><br>

  ![GCP Configuration empty](/images/ee/gcp-setup/gcp-configure-empty.png)

- Complete the fields, keeping in mind the following guidelines:
  - Supply a descriptive name that preferably contains Google or GCP, which is especially important if you are planning to configure other providers.

  - If your YugabyteDB Anywhere instance is not running inside GCP, you need to supply YugabyteDB Anywhere with credentials to the desired GCP project by uploading a configuration file. To do this, select **Upload Service Account config** in the **Credential Type** field and proceed to upload the JSON file that you obtained when you created your service account, as described in [Prepare the Google Cloud Platform (GCP) environment](../../../install-yugabyte-platform/prepare-environment/gcp).<br>

    If your YugabyteDB Anywhere instance is running inside GCP, the preferred method for authentication to the GCP APIs is to add a service account role to the GCP instance running YugabyteDB Anywhere and then configure YugabyteDB Anywhere to use the instance's service account. To do this, select **Use Service Account on instance** in the **Credential Type** field.

  - If this is a new deployment, it is recommended that you use the **VPC Setup** field to create a new VPC specifically for YugabyteDB nodes. You have to ensure that the YugabyteDB Anywhere host machine can connect to your Google Cloud account where this new VPC will be created.

    <br>Alternatively, you may choose to specify an existing VPC for YugabyteDB nodes, in which case you would need to map regions by providing the following:

    - A region name.
    - A subnet ID.
    - Optionally, a custom machine image. YugabyteDB Anywhere allows you to bring up universes on Ubuntu 18.04 host nodes, assuming you have Python 2 or later installed on the host, as well as the provider created with a custom AMI and custom SSH user.

    \
    The third option that is available only when your YugabyteDB Anywhere host machine is also running on Google Cloud, is to use the same VPC on which the YugabyteDB Anywhere host machine runs. Note that choosing to use the same VPC as YugabyteDB Anywhere is an advanced option, which assumes that you are in complete control over this VPC and will be responsible for setting up the networking, SSH access, and firewall rules for it.

- Click **Save** and wait for the configuration to complete.

  This process includes generating a new VPC, a network, subnetworks in all available regions, as well as a new firewall rule, VPC peering for network connectivity, and a custom SSH key pair for YugabyteDB Anywhere-to-YugabyteDB connectivity.

Upon completion, you should see the configuration similar to the following:

![GCP Configuration success](/images/ee/gcp-setup/gcp-configure-success.png)

Now you are ready to create a YugabyteDB universe on GCP.