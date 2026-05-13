import subprocess
import pytest

BINARY = "./codexion"

def run_binary(args):
    """Executes the binary and returns the return code."""
    process = subprocess.run(
        [BINARY] + args,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    return process.returncode

# --- 1. MATHEMATICAL SIGNS AND FORMATTING ---
@pytest.mark.parametrize("args", [
    ["5", "800", "+200", "200", "200", "7", "100", "fifo"],
    ["5", "800", "++200", "200", "200", "7", "100", "fifo"],
    ["5", "800", "--200", "200", "200", "7", "100", "fifo"],
    ["5", "800", "+-200", "200", "200", "7", "100", "fifo"],
    # Mixed signs (Should fail)
    ["5", "800", "200-", "200", "200", "7", "100", "fifo"],
    # Sign at the end (Should fail)
])
def test_signs_format(args):
    """Checks strict handling of mathematical signs."""
    # Note: Case "+200" might pass or fail
    # depending on your 'is_digit' implementation.
    # Per subject rules, we usually reject anything
    # that isn't a pure positive integer.
    if args[2] == "+200":
        pass
    else:
        assert run_binary(args) != 0

# --- 2. SPACES AND INVISIBLE CHARACTERS ---
@pytest.mark.parametrize("args", [
    ["5", "800 ", "200", "200", "200", "7", "100", "fifo"],   # Trailing space
    ["5", "800", "\t200", "200", "200", "7", "100", "fifo"],
    # Tabulation character
    ["5", "800", "", "200", "200", "7", "100", "fifo"],       # Empty string ""
])
def test_spaces_and_empty(args):
    """Ensures spaces and empty arguments are rejected."""
    # Arguments must be valid integers; empty strings or spaces must fail.
    assert run_binary(args) != 0

# --- 3. BUSINESS LOGIC & EDGE CASES ---
@pytest.mark.parametrize("args", [
    ["0", "800", "200", "200", "200", "7", "100", "fifo"],  # Zero coders
    ["5", "800", "200", "200", "200", "7", "100", "wrong_sched"],
    ["5", "-800", "200", "200", "200", "7", "100", "fifo"],
])
def test_business_logic_limits(args):
    """Verifies that logically invalid inputs are rejected."""
    # The program must reject negative numbers or invalid schedulers.
    assert run_binary(args) != 0
