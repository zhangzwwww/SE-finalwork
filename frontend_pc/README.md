# Frontend pc

This folder gives the front end implemention of the project.

## Notes

`./frontend_pc.pro` is the QT parent project file. In the QT creator will build both `./app/app.pro` and `./test/test.pro`. Thus, please remember not to change the relative path of these two files during development.

`./app/` is the source code of the project. For now the team has two ways of building the project. 

- If you are using `cmake` , construct your `CMakeLists.txt` in `./` or `./App/` . Remember set the building output path in a saparate folder, for example `./App/build/`. 
- If you are using QT creator and `qmake` to build projects, just remember to check the building output path. When building, remember to check the source code path in project files, such as `./app/app.pro`. Further, as different platforms may have different encoding for Chinese. Plz set the encoding mode of your editor as `GBK`, if there's something wrong when displaying Chinese.

Again, be careful when changing the path of files. Rember to ensure the project building works when doing so.



## Usage of Test framework

`src/test/test.pro` is our test framework

- if you use `qmake` in `qt creator` , bin file will be `build` folder

- if you want to add a new test class
  1. add new `.h & .cpp` file of you test class, eg: `test_login.h, test_login.cpp`
  2. Modify `test.pro` ,add your new file path ( if you use `qt creator`, it is automatic)
  3. Modify `test_main.cpp`, add `TEST_CLASS(YourClassName);`, eg: `TEST_CLASS(TestLogin)`'