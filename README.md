# slog (small log)

## Development Environment 

This repository can and shoul be developed and tested using a docker container. This way we control the environment, tools and version used.

In this repo there is a dockerfile that should be used to create a docker image that then can be used as base to the development docker container. 

Follow the steps below to get the environment ready:

1. Intall docker in your computer
2. In the same path of the 'Dockerfile' (repository root) execute the command bellow. 
   - The option -t defines the name and version of the image to be created (choose one that you like or use the one in the example). 
   - The '.' means we should generate the image from the Dockerfile in the current directory.
    ```
    docker build -t slog-docimg:1.0 .
    ```
3. Once the cmmand above finished you should have the new image ready to use in you machine. Use the following command to see all the images available in your machine.
    ```
    docker images
    ```
4. Now we should run the image with the command below.
   - -it means we will run the container in interactive mode, meaning the container will keep running until we stop it
   - -v maps the container folder `/home` to a directory in your host machine. This way you keep your data  even when you delete the container. Its a good idea to develop all your work inside `/home` directory.
   - --name defines a name for your new container.
    ```
    docker run -it -v C:\Users\lcrgo\Downloads\docker_volumes\slog:/home --name slog_container slog-docimg:1.0
    ```
5. Log in the container and go to home directory
    ```
    cd /home
    ```
6. Clone the repository there
    ```
    git clone https://github.com/luisgonc/slog.git
    ```
7. Move into the repo folder and create build directory
    ```
    cd slog & mkdir build & cd build
    ```
8. Configure the project by running the cmake command in the parent directory.
    ```
    cmake ..
    ```
9. Compile the project
    ```
    cmake --build .
    ```
10. Run the unit test executable an check the results
    ```
    ./unit_tests
    ```