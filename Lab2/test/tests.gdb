# Test file for "Lab2"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n
# Tests
test "PINA: 0x20, PINB: 0x20, PINC: 0x20 => PORTC: 0x60"
setPINA 0x20
setPINB 0x20
setPINC 0x20
continue 5
expectPORTD 0x60
printPORTD
checkResult

test "PINA: 0x20, PINB: 0x20, PINC: 0x20 => PORTC: 0x60"
setPINA 0x40
setPINB 0x40
setPINC 0x40
continue 5
expectPORTD 0xc1 
printPORTD
checkResult

test "PINA: 0x80, PINB: 0x10, PINC: 0x10 => PORTC: 0x60"
setPINA 0x80
setPINB 0x10
setPINC 0x10
continue 5
expectPORTD 0xA3 
printPORTD
checkResult

test "pina: 0x50, pinb: 0x10, pinc: 0x00 => portc: 0x60"
setPINA 0x00
setPINB 0x00
setPINC 0x80
continue 5
expectPORTD 0x82
printPORTD
checkResult

test "pina: 0x50, pinb: 0x10, pinc: 0x00 => portc: 0x60"
setPINA 0x60
setPINB 0x60
setPINC 0x60
continue 5
expectPORTD 0x21
printPORTD
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
