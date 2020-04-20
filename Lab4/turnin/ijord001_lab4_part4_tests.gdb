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
test "Presses wrong code - PINA: 0x00 => PORTC: 0x00"
set state = START
setPINA 0x02
continue 2
expectPORTC 0x00
checkResult

test "Presses right code - PINA: 0x04, 0x02 => PORTC: 0x01"
set state = START
setPINA 0x04
continue 2
setPINA 0x02
continue 5
expectPORTC 0x01
checkResult


test "Presses wrong code - PINA: 0x04, 0x01 => PORTC: 0x00"
set state = START
setPINA 0x04
continue 2
setPINA 0x01
continue 2
expectPORTC 0x00
checkResult


test "Presses right code without pound - PINA: 0x02, 0x02 => PORTC: 0x00"
set state = START
setPINA 0x02
continue 2
setPINA 0x02
continue 2
expectPORTC 0x00
checkResult

test "Presse the right buttons at the same time - PINA: 0x06 => PORTC: 0x00"
set state = START
setPINA 0x06
continue 2
expectPORTC 0x00
checkResult

test "Unlocks then Locks - PINA: 0x04, 0x02, 0x04,0x02 => PORTC: 0x00"
set state = START
setPINA 0x04
continue 2
setPINA 0x02
continue 5
setPINA 0x04
continue 2
setPINA 0x02
continue 5
expectPORTC 0x00
checkResult

test "Unlocks then fails to Lock - PINA: 0x04, 0x02, 0x04,0x02 => PORTC: 0x01"
set state = START
setPINA 0x04
continue 2
setPINA 0x02
continue 5
setPINA 0x04
continue 2
setPINA 0x01
continue 5
expectPORTC 0x01
checkResult



# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
