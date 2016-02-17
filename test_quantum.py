# -*- coding: utf-8 -*-

import unittest
from subprocess import check_output


def run(json):
    return check_output(["./quantum", "jsons/%s" % json]).strip()


class Quantum_Test(unittest.TestCase):

    def test_print_int(self):
        self.assertEqual("नमस्ते, Hello, Привет, 您好, ہیلو", run("print.json"))

    def test_add_floats(self):
        self.assertEqual("44.21", run("add.json"))

if __name__ == '__main__':
    unittest.main()
