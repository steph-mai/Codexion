import subprocess
import pytest

BINARY = "./codexion"

def run_binary(args):
    process = subprocess.run(
        [BINARY] + args,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    return process.returncode

# --- 1. TESTS DE SIGNES ET FORMATS BIZARRES ---
@pytest.mark.parametrize("args", [
    ["5", "800", "+200", "200", "200", "7", "100", "fifo"],   # Signe + (devrait passer)
    ["5", "800", "++200", "200", "200", "7", "100", "fifo"],  # Double signe (doit fail)
    ["5", "800", "--200", "200", "200", "7", "100", "fifo"],  # Double moins (doit fail)
    ["5", "800", "+-200", "200", "200", "7", "100", "fifo"],  # Signes mixtes (doit fail)
    ["5", "800", "200-", "200", "200", "7", "100", "fifo"],   # Signe à la fin (doit fail)
])
def test_signs_format(args):
    """Vérifie la gestion rigoureuse des signes mathématiques."""
    # Le premier cas (+200) peut passer ou fail selon ta stricte application de is_number_str
    # Si is_number_str n'autorise que '0'-'9', alors +200 doit fail.
    if args[2] == "+200":
        pass # Dépend de ta consigne, souvent on accepte le '+' unique
    else:
        assert run_binary(args) != 0

# --- 2. TESTS D'ESPACES ET CARACTÈRES INVISIBLES ---
@pytest.mark.parametrize("args", [
    ["5", "800 ", "200", "200", "200", "7", "100", "fifo"],   # Espace après
    ["5", "800", "\t200", "200", "200", "7", "100", "fifo"],  # Tabulation
    ["5", "800", "", "200", "200", "7", "100", "fifo"],       # Argument vide ""
])
def test_spaces_and_empty(args):
    """Vérifie que les espaces et arguments vides ne passent pas inaperçus."""
    #is_number_str devrait normalement rejeter tout ce qui n'est pas un chiffre pur
    assert run_binary(args) != 0

# --- 3. TESTS DE LOGIQUE MÉTIER AVANCÉE (VALEURS LIMITES) ---
@pytest.mark.parametrize("args", [
    ["0", "800", "200", "200", "200", "7", "100", "fifo"],           # Zéro codeur (Interdit)
    ["2001", "800", "200", "200", "200", "7", "100", "fifo"],         # Trop de codeurs (ex: > 200)
    ["5", "0", "200", "200", "200", "7", "100", "fifo"],             # Time to burnout à 0
    ["5", "800", "200", "200", "200", "7", "99999999999999999999", "fifo"],    # Dongle cooldown immense
])
def test_business_limits(args):
    """Vérifie que validate_args rejette les valeurs physiquement impossibles."""
    assert run_binary(args) != 0

# --- 4. LE CAS LLONG_MIN (TRÈS SPÉCIFIQUE) ---
def test_llong_min():
    """Vérifie le comportement face à la plus petite valeur possible d'un long long."""
    args = ["5", "-9223372036854775808", "200", "200", "200", "7", "100", "fifo"]
    assert run_binary(args) != 0
