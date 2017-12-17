# 2018-PowerUp
5333 &amp; 5663 code base for 2018 FIRST POWER UP

## Getting Started
*if you're on mac or linux, you will have to run `chmod +x gradlew` before working*  
Make sure to read 'Making Changes' to get your repository set up first!  

1. Install the Toolchain (`./gradlew :common:installToolchain`)  
2. Run a build (`./gradlew :5333:build` or `./gradlew :5663:build`)  
3. Run a deploy (if you're connected to the robot) (`./gradlew :5333:deploy` or `./gradlew :5663:deploy`)  

You can view the shuffleboard interface with `./gradlew :common:shuffleboard`  
You can view the robot's console output with `./gradlew :5333:riolog -Pfakeds` or `./gradlew :5663:riolog -Pfakeds`

## Making Changes
***initial setup (do once)***  
1\. Fork this repository to your own account (top right corner of github page)  
2\. Clone your repository fork (File -> Clone Repository -> yourname/2018-PowerUp)  

***before you make a change***  
3. Switch to 'master' branch  
4. Select 'Fetch origin'  
5. Select Branch (from the toolbar) -> Merge into current branch.. -> upstream/master  
6. Create a new branch (name it yourname/yourfeature)  

***make and commit your changes***  
7. Select Branch (from the toolbar) -> Create pull request (select publish branch if needed)  
8. Publish your Pull Request with some information. It will be reviewed and merged.