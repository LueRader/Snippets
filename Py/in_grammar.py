class Production:
    def __init__(self, left, right):
        self.left  = left
        self.right = right

class Grammar:
    def __init__(self, vari, term, prods, init):
        self.variables = vari
        self.terminal  = term
        self.prod_list = prods
        self.ini       = init


def apply_production(input_string, production):
    results = []
    idx = input_string.find(production.left)
    while idx >= 0:
        results.append(input_string[:idx] + production.right + input_string[idx+1:])
        idx = input_string.find(production.left, idx+1)
    return results
    
    
def apply_all_productions(input_strings, productions):
    results = []
    for input_str in input_strings:
        for prod in productions:
            res = apply_production(input_str, prod)
            if res != []:
                results.extend(res)
    return results

def is_producable(grammar, input_string):
    curr = [grammar.ini]
    while True:
        curr = apply_all_productions(curr, grammar.prod_list)
        if input_string in curr:
            print("Yay")
            return True

        if not any(len(x) <= len(input_string) for x in curr):
            print("Nay")
            return False

is_producable(Grammar(["S"], ["a", "b"], [Production("S", "aSbS"), Production("S", "ab")], "S"), "aabbab")
