# Server

## Project Structure

* register

Project choose consul as service registry component.

* config

config.yaml is the config file of server side, however in order not to expose the data of real config, config.yaml.swap is used to track the structure of config.yaml, developers can maintain your own config file in your local environment and update config.yaml.swap if changed.
