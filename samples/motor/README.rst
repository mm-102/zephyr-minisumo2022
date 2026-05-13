.. zephyr:code-sample:: motor
   :name: H-Bridge Motor Driver
   :relevant-api: pwm_interface gpio_interface

   Control an H-Bridge motor driver using PWM and GPIO subsystems.

Overview
********

A simple motor control demo showcasing the use of a custom H-Bridge motor driver 
with Zephyr's PWM and GPIO APIs. 

The sample initializes two motors (left and right) using devicetree macros, drives 
them forward at 60% power, executes a right turn using skid steering (left forward, 
right backward), comes to a full stop, and then enters a continuous loop that 
alternates the left motor's direction.

Requirements
************

The board hardware must have two device nodes capable of driving the H-Bridge inputs. 
These nodes must be compatible with ``sims,hbridge-motor`` and use the devicetree 
node labels ``motor1`` and ``motor2``.

Each motor node requires the following properties:

* ``pwms``: Two PWM channels corresponding to the L1 and R1 inputs.
* ``pwm-names``: String array naming the PWMs, which must be ``"l1"``, ``"r1"``.
* ``l2-gpios``: A GPIO pin assigned for the L2 input.
* ``r2-gpios``: A GPIO pin assigned for the R2 input.

An overlay file (e.g., ``app.overlay``) is typically required to map these 
properties to the specific pins of your target board.

Building and Running
********************

This sample can be built for multiple boards, provided the required devicetree 
nodes are defined in the board's devicetree or a provided overlay. In this example, 
we will build it for a hypothetical board named ``my_custom_board``:

.. zephyr-app-commands::
   :zephyr-app: samples/motor
   :board: my_custom_board
   :goals: build
   :compact:

Once flashed, you should observe the motors executing the programmed movement sequence 
and see the progression logged to the console:

.. code-block:: console

    [00:00:00.000,000] <inf> main: Starting Robot Motor Demo...
    [00:00:00.005,000] <inf> main: Motors ready. Moving forward...
    [00:00:02.005,000] <inf> main: Turning right (skid steer)...
    [00:00:03.005,000] <inf> main: Full Stop.