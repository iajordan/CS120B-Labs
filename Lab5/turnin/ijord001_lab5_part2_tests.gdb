# Test file for "Lab4"


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
test "Doesnt Press buttons - PINA: 0xFF => PORTC: 0x00"
set state = START
setPINA 0xFF
continue 3
expectPORTC 0x00
checkResult

test "Press buttons once - PINA: 0xFE => PORTC: 0x01"
set state = START
setPINA 0xFE
continue 2
setPINA 0xFF
continue 2
expectPORTC 0x01
checkResult

test "Press INC 5 times then DEC 2 times"
set state = START
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFD
continue 3
setPINA 0xFF
continue 3
setPINA 0xFD
continue 3
setPINA 0xFF
continue 3
expectPORTC 0x03
checkResult

test "Press INC 3 times then both to clear"
set state = START
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFE
continue 3
setPINA 0xFF
continue 3
setPINA 0xFC
continue 3
setPINA 0xFF

expectPORTC 0x00
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
