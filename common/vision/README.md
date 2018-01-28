Vision System Setup (First-Run) Instructions
====

1. Image a Raspberry Pi with Raspbian. Either headless or desktop environment is up to you.

2. Set the hostname of the Pi to either `pi_5333` or `pi_5663` depending on your team (done through `sudo raspi-config`)

3. Enable SSH on the Pi through `sudo raspi-config`

4. Reboot.

5. Run `./gradlew :common:vision:deploy`. 

6. SSH into the Pi and run `cd /home/pi/setup; ./setupOpencv.sh`. This will take a while.