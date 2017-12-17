# 2018-PowerUp
5333 &amp; 5663 code base for 2018 FIRST POWER UP

## Getting Started
*if you're on mac or linux, you will have to run `chmod +x gradlew` before working*  

1. Install the Toolchain (`./gradlew :common:installToolchain`)  
2. Run a build (`./gradlew :5333:build` or `./gradlew :5663:build`)  
3. Run a deploy (if you're connected to the robot) (`./gradlew :5333:deploy` or `./gradlew :5663:deploy`)  

You can view the shuffleboard interface with `./gradlew :common:shuffleboard`  
You can view the robot's console output with `./gradlew :5333:riolog -Pfakeds` or `./gradlew :5663:riolog -Pfakeds`

