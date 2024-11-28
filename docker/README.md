# Webots & ROS (1) Inside Docker

Docker is an open-source platform designed to simplify the development, deployment, and operation of applications by using containerization. Consisting of Docker Engine, a portable, lightweight runtime and packaging tool, and Docker Hub, a cloud service for sharing applications and automating workflows, Docker enables apps to be quickly assembled from components and eliminates the friction between development, QA, and production environments.

---

## Why Use Docker?

Since ROS 1 does not support Ubuntu 22.04 or newer OS versions, and Webots R2024a has dropped support for Ubuntu 20.04 and ROS 1, this setup ensures a unified development environment. It allows ROS 1 and Webots to coexist seamlessly, bypassing OS and Webots version restrictions. Also. with this setup, you can use GUI applications directly from Docker, such as RViz, with GPU acceleration enabled.

---

## Learn more about Docker:
- [Official Docker Documentation](https://docs.docker.com/)
- [Docker CLI Reference](https://docs.docker.com/engine/reference/commandline/docker/)

---

## Dependencies

```bash
sudo apt install git make curl
curl -sSL https://get.docker.com | sh && sudo usermod -aG docker $USER
```

## Use Docker with Webots and ROS 1:

This guide explains how to set up and run Docker with Webots and ROS 1 using the `webots_ros` repository.

## Steps

1. Clone the `webots_ros` repository:

    ```bash
    cd ~
    git clone https://github.com/cyberbotics/webots_ros.git
    ```

2. Navigate into the `docker` directory:

    ```bash
    cd ~/webots_ros/docker
    ```

3. Build, run, and attach a shell to the running container (only required once; update the Webots version if needed):

    ```bash
    WEBOTS_VERSION=2023b make build run exec
    ```

    - `build`: Builds the Docker image.
    - `run`: Starts the Docker container.
    - `exec`: Attaches a shell to the running container.

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
    roslaunch webots_ros tiago.launch
    ```
7.  Attach a new shell to the running container and run Rviz:

    ```bash
    make exec
    rviz -d ~/ros_ws/src/webots_ros/config/rviz_config.rviz
    ```
8.  Attach a new shell to the running container and teleoperate the tiago robot:

    ```bash
    make exec
    rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=/diff_drive_controller/cmd_vel
    ```

## Notes:

For more information on how to run ROS 1 and Webots, follow the instructions at:

- [Sample Simulations](https://wiki.ros.org/webots_ros/Tutorials/Sample%20Simulations)
- Ensure you have Docker installed before proceeding
