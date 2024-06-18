add_executable(CRNLCHCLITest CLITests.cpp ./../src/CLI/CRNLCHCLI.cpp)
target_link_libraries(CRNLCHCLITest CRNLCH clipp)