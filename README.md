# slog (small log)

This is a small logging library coded in c++ aiming the embedded development in small microcontrollers. 

Althogh it is done in c++ it is not taking advantage of all c++ features and libraries, for example it do not make use of heap because in some system that is something we can't afford also due to determinism.

Check below the approach taken to develop and test the library code and also how you shoul use it in you own project.

## Development Environment 

This repository can and shoul be developed and tested using a docker container. This way we control the environment, tools and version used.

In this repo there is a dockerfile that should be used to create a docker image that then can be used as base to the development docker container. 

Follow the steps below to get the environment ready:

1. Intall docker in your computer
2. In the same path of the 'Dockerfile' (repository root) execute the command bellow. 
   - The option `-t` defines the name and version of the image to be created (choose one that you like or use the one in the example). 
   - The `.` means we should generate the image from the Dockerfile in the current directory.
    ```
    docker build -t slog-docimg:1.0 .
    ```
3. Once the cmmand above finished you should have the new image ready to use in you machine. Use the following command to see all the images available in your machine.
    ```
    docker images
    ```
4. Now we should run the image with the command below.
   - `-it` means we will run the container in interactive mode, meaning the container will keep running until we stop it
   - `-v` maps the container folder `/home` to a directory in your host machine. This way you keep your data  even when you delete the container. Its a good idea to develop all your work inside `/home` directory.
   - `--name` defines a name for your new container.
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

---
---

## Using the library

As a complement to the usage description below you can always refer to the test code in the `test` folder.

### Intatiating the logger
Create a static instance of the logger as it most probably will live as long as the program. When need to provide
a logger name at creation time, the name will destinguish the logger from all the others that may exist (you can have as many logger as needed). 

The logger name is always printed together with the logger message.
``` 
static slog::logger logger = slog::logger("logger_name");
```

### Set logger level
By default the logger is created with `INFO` level, but that can be changed dinamically at runtime with the method below.
``` 
logger.set_Level(slog::logger::level::DEBUG);
```
It also possible to check the current logger level: `auto current_level = logger.get_Level()`


### Add appenders
By default loggers are created with NO appenders, so we must provide at least one otherwise the logger is useless. Appenders are callback function that are provided to the library and will be called when some log message was passed to the logger. The appenders are the function that know what to do with the messages, like write them to memory, to the console, send it to an external service, etc.

Appender function must have the following signature `void appender_fn(const char*)`
``` 
logger.add_appender(appender_fn);
```

### Set time provider
By default there is no time provider set when the logger is created, so we must provide one otherwise log messages wont have any timestamp.
Time provider function is provided to the library as a callback function and should return the current time and data when called.

Time provider function should have the following signature `slog::timedate time_provider_fn()`
```
logger.set_time_provider(time_provider_fn);
```

By default only the time (hour:minute:second.millisecond) is printed with the message. If is also required the date we need to enable it with the function `logger.set_print_date(true);`

### Logging messages
To log a message we should use the function below with 2 parameters. The first is the log level of the message, the second is the message it self. currently only 3 types are accepted:
 - char *;
 - std::string;
 - std::string_view;

```
logger.log(slog::logger::level::INFO, "log message test");
```
By calling the function above the following will be printed:
```
[2024/02/10 11:47:22:568][INFO ][logger_name] log message test
```

In the case the message is composed by other variables like integers, chrono etc we can use the `<<` operator to pass them. Currently the following types are supported by the `<<` operator:
 - const char*
 - std::string
 - std::string_view
 - std::chrono::seconds
 - std::chrono::milliseconds
 - std::chrono::microseconds
 - radix (slog::logger::radix::BIN / OCT / DEC / HEX)
 - Integral types (integer values)

 Check the exmample below
 ```
 logger.log(slog::logger::level::INFO, "Operator << ") << slog::logger::radix::BIN << 170;
 ```
 this will print the following: `"\n[23:25:16.753][INFO ][test_logger] Operator << 0b10101010"`