# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.25
cmake_policy(SET CMP0009 NEW)

# MILA_OUTPUTS at CMakeLists.txt:59 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES false "/home/stefan/FIT/PJP/slavkste/tests/run/*.run[0-9]*.out")
set(OLD_GLOB
  "/home/stefan/FIT/PJP/slavkste/tests/run/arrayMax.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/arrayTest.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/arrayTest.run2.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/arrayTest.run3.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/arrayTest.run4.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/breakContinue.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/consts.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/explicitCast.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions.run2.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions.run3.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions.run4.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions2.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions2.run2.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/expressions2.run3.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/factorial.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/factorialRec.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/factorialRec.run2.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/factorialRec.run3.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/factorization.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/fibonacci.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/gcd.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/implicitCast.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/indirectrecursion.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/inputOutput.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/inputOutput.run2.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/inputOutput.run3.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/isprime.run1.out"
  "/home/stefan/FIT/PJP/slavkste/tests/run/sortBubble.run1.out"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/stefan/FIT/PJP/slavkste/build/CMakeFiles/cmake.verify_globs")
endif()

# MILA_SOURCES at CMakeLists.txt:48 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/stefan/FIT/PJP/slavkste/samples/*.mila")
set(OLD_GLOB
  "/home/stefan/FIT/PJP/slavkste/samples/arrayMax.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/arrayTest.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/breakContinue.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/consts.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/explicitCast.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/expressions.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/expressions2.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/factorial.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/factorialCycle.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/factorialRec.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/factorization.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/fibonacci.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/gcd.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/implicitCast.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/indirectrecursion.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/inputOutput.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/isprime.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/lexerTest.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/operatorPrecedence.mila"
  "/home/stefan/FIT/PJP/slavkste/samples/sortBubble.mila"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/stefan/FIT/PJP/slavkste/build/CMakeFiles/cmake.verify_globs")
endif()
