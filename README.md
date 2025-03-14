# Robot Squad

Robot Squad is a game where you control a bomb-squad robot tasked with navigating a hazardous warehouse. Your mission: collect bombs scattered throughout the warehouse and safely transport them to the designated parking lot.

**Key Features:**

* Control a robot through a 3D warehouse environment.
* Locate and collect scattered bombs.
* Safely transport bombs to the parking lot.
* Built using OpenGL for graphics rendering.
* Physics powered by Bullet3.

## Getting the Code

To get the code and play Robot Squad, follow these steps:

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/ARPP3/RobotSquad.git
    ```

2.  **Navigate to the project directory:**

    ```bash
    cd RobotSquad
    ```

3.  **Initialize and update submodules:**

    The project uses Bullet3 as a submodule for physics calculations. To ensure you have all the necessary code, run the following command:

    ```bash
    git submodule update --init --recursive
    ```
