# webots_ros

[![Build Status](http://build.ros.org/buildStatus/icon?job=Mbin_uB64__webots_ros__ubuntu_bionic_amd64__binary&subject=Noetic)](http://build.ros.org/job/Mbin_uB64__webots_ros__ubuntu_bionic_amd64__binary/)

The webots_ros package contains examples for interfacing ROS nodes with the standard ROS controller of Webots.

How to set-up the ROS interface in Webots:
  - https://www.cyberbotics.com/doc/guide/tutorial-9-using-ros
ROS tutorial for Webots:
  - https://www.cyberbotics.com/doc/guide/using-ros

## What is Docker?

Docker is an open-source platform designed to simplify the development, deployment, and operation of applications by using containerization. Consisting of Docker Engine, a portable, lightweight runtime and packaging tool, and Docker Hub, a cloud service for sharing applications and automating workflows, Docker enables apps to be quickly assembled from components and eliminates the friction between development, QA, and production environments. As a result, IT can ship faster and run the same app, unchanged, on laptops, data center VMs, and any cloud. Containers allow developers to package an application with all its dependencies into a standardized unit for software development, making it easy to run applications in any environment.

### Key Features:
- **Lightweight**: Containers share the host OS kernel, making them faster and more resource-efficient than virtual machines.
- **Portability**: Containers can run consistently across different environments, such as development, testing, and production.
- **Isolation**: Each container runs in its own isolated environment, ensuring no interference between applications.

---

## Why Use Docker?

1. **Simplified Development**: Docker ensures that your application runs the same in development and production environments.
2. **Dependency Management**: Bundle your application with its dependencies into a single container.
3. **Rapid Deployment**: Quickly start, stop, and replicate containers across environments.
4. **Efficient Resource Usage**: Docker containers are lightweight compared to traditional virtual machines.

---

## Learn more about Docker:
- [Official Docker Documentation](https://docs.docker.com/)
- [Docker CLI Reference](https://docs.docker.com/engine/reference/commandline/docker/)

---

## Use Docker with Webots and ROS 1:

This guide explains how to set up and run Docker with Webots and ROS 1 using the `webots_ros` repository.

### Steps

1. Clone the `webots_ros` repository:

    ```bash
    https://github.com/cyberbotics/webots_ros.git
    ```

2. Navigate into the `docker` directory:

    ```bash
    cd ~/webots_ros/docker
    ```

3.  Run the command:

    ```bash
    make build run exec
    ```

4.  Build the ROS workspace:

    ```bash
    catkin_make
    ```
5. Source the environment:

    ```bash
    source devel/setup.bash
    ```
6.  Run a simulation with an example launch file:

    ```bash
    roslaunch webots_ros e_puck_line.launch
    ```

### Notes:

For more information on how to run ROS 1 and Webots, follow the instructions at:

- [Sample Simulations](https://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations)
- Ensure you have Docker installed before proceeding

---

## Acknowledgement

<a href="http://rosin-project.eu">
  <img src="http://rosin-project.eu/wp-content/uploads/rosin_ack_logo_wide.png"
       alt="rosin_logo" height="60" >
</a></br>

Supported by ROSIN - ROS-Industrial Quality-Assured Robot Software Components.
More information: <a href="http://rosin-project.eu">rosin-project.eu</a>

<img src="http://rosin-project.eu/wp-content/uploads/rosin_eu_flag.jpg"
     alt="eu_flag" height="45" align="left" >

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement no. 732287.
