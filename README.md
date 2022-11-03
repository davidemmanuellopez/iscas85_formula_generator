# ISCAS85_formula_generator
A C++ utility for generate propositional non-clausal fórmulas, using ISCAS85 circuit specification.

    Copyright (C) <2022  David Emmanuel Lopez
    Contact: david.emmanuel.lopez@gmail.com
    http://www.github.com/davidemmanuellopez/iscas85_formula_generator
 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Goal

The creation of this utility is intended for provide test cases for non-clausal SAT solvers, or non-clausal Automatic Demonstrators. The available test cases on SAT competitions or other sources are in clausal form (CNF, DNF). Thats dificult to test a non-clausal SAT solver, because a non-clausal form can allow algorithmic improvements that cannot be used in the input formula is always clausal. Also, a random formula generation does not represent the complexity of the problematique. The ISCAS85 cases represent combinational complexity on circuits, reference: https://www.davidkebo.com/documents/iscas85.pdf

The utility was created by me for test Demuba3 Automatic Demonstrator, in https://github.com/davidemmanuellopez/demuba3. But can be used for any solver, only is need to use the same formula representation or modify the source code to generate formula with other nomenclature.

## Use

* First, compile de utilities with compile.sh, or compile with command line the two single sources.
* Create a subdirectory called "tests", with mkdir ./tests
* run "./formula_generator.sh [test_case]", for example "./formula_generator.sh c17"

## Explanation

The script takes the test case passed as argment and assume the existence of "./iscas85/[test_case]". Then, perform a preproccesing thats remove spaces and creates [test_case].clean file. This file is used to generate formulas in directory "./tests/[test_case]/". The formulas are not uploaded because the size can be in MB.

The generator will create one formula per circuit output, then a iscas85 test case creates a set of formulas.

## Fórmula format

* Each proposional variable have "p" preffix, and is identified by a integer number. Like p1, p2, p3, pi.
* Or operator is specified by "v" character.
* And operator is specified by "&" character.
* Not operator is specified by "-" character.
