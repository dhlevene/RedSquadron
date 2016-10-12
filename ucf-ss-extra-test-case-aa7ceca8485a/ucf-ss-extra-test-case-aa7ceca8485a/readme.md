# COP3402-16Fall 0001 Systems Software
## Extra Test Cases

This public repo provide some extra test cases as well as some scripts to automate the testing process.

## Official Information
[course repo](https://bitbucket.org/schneider128k/2016_fall_2016_cop3402.git)
[course repo wiki](https://bitbucket.org/schneider128k/2016_fall_2016_cop3402/wiki.git)

## Installation
1. Download this project
It is recommended to add this repo as submodule to your project repo
```
mkdir -p submodule && cd $_ # using subfolder is optional
git submodule add https://bitbucket.org/ucf-ss/extra-test-case
```
2. cd into the submodule
```
cd extra-test-case
```
3. Set up the configuration
```
./configure
```

## Usage
### 1. test with extra test-case
format : bin/gen-output-[program].sh
e.g. ```bin/gen-output-lexer.sh```
### 2. compare against my output
format : bin/test-[program].sh
e.g. ```bin/test-lexer.sh```

## Details
### project-0
[vm](doc/vm.md)
### project-1
[lexer](doc/lexer.md)
