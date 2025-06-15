# Tracer

This project is a simple implementation of the "[Ray Tracing in One Weekend](https://raytracing.github.io/)" book series. This implementation uses several external libraries for various core functionalities:

  - **[GLM (OpenGL Mathematics)](https://github.com/g-truc/glm)**: Used for mathematical operations.
  - **[GLFW](https://www.glfw.org/)**: Employed to display the final rendered image in a window.
  - **[Glad](https://glad.dav1d.de/)**: Utilized for loading OpenGL function pointers.
  - **[stb\_image](https://github.com/nothings/stb)**: A single-file library for image loading and saving.

## Project Setup

Follow these instructions to get a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

You will need the following software installed to build and run this project:

  - A C++ compiler with support for C++11 or newer (this project was developed using Visual Studio 2017).
  - [Premake](https://premake.github.io/): A build configuration tool used to generate project files.

### Installation

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/dnikolaidis2/Tracer
    ```

2.  **Navigate to the project directory:**

    ```bash
    cd Tracer
    ```

3.  **Initialize and update the Git submodules:**

    ```bash
    git submodule init
    git submodule update
    ```

4.  **Generate the project files.** Use Premake to generate the solution/project files for your development environment. For example, to generate a Visual Studio 2022 solution, run the following command:

    ```
    premake5 vs2022
    ```