/*
    A Automatic Demonstrator written in Prolog
    Copyright (C) <2022  David Emmanuel Lopez
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
*/

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <algorithm>

using namespace std;
class Gate;
static set<Gate*> pisados;
map<string,Gate*> mapa;
static int deep=0;

string reverseGet(map<string,Gate*> mapa, Gate* g)
{
	map<string,Gate*>::iterator it = mapa.begin();
	while (it->second!=g && it!=mapa.end()) it++;
	return it->first;
	};
	
list<string> split(string input, string delimiter)
{
	list<string> l;
	size_t pos = 0;
	string token;
	while ((pos = input.find(delimiter))!=string::npos)
	{
		token = input.substr(0,pos);
		l.push_back(token);
		input.erase(0,pos+ delimiter.length());
		};
	l.push_back(input);
	return l;
	};
	
class Gate
{
	public:
		virtual void convert(ofstream &output_file) {};
	};

class GateRecursive: public Gate
{
public:
	virtual void addGate(Gate* g) {terms.push_back(g);};
protected:
    list<Gate*> terms;
};

class Input: public Gate
{
	private:
	string id;
	public:
		Input(string v):id(v){};
		void convert(ofstream &output_file) { output_file << "p" << id; };
	};

class Or: public GateRecursive
{
	public:
	void convert(ofstream &fsalida)
	{
		if (pisados.find(this)==pisados.end())
		{
			cout << "pisados: " << pisados.size() << "/" << mapa.size() << endl;
			pisados.insert(this);
			} 
				
		string salida;
		list<Gate*>::iterator it = terms.begin();
		Gate* g = *it;
        g->convert(fsalida);
		it++;
		while (it!=terms.end())
		{
			g = *it;
			fsalida << " v "; 
			deep++;
            g->convert(fsalida);
			deep--;
			it++;
			};
	};
			
};

class NOr: public Or
{
	void convert(ofstream &fsalida)
		{
			fsalida << "-(";
            Or::convert(fsalida);
			fsalida << ")";
		}
	};

class And: public GateRecursive
{
public:
	void convert(ofstream &fsalida)
	{
        if (pisados.find(this)==pisados.end())
		{
			cout << "pisados: " << pisados.size() << "/" << mapa.size() << endl;
			pisados.insert(this);
			} else
		
		string salida;
		list<Gate*>::iterator it = terms.begin();
		Gate* g = *it;
		fsalida << "(";
        g->convert(fsalida);
		fsalida << ")";
		it++;
		while (it!=terms.end())
		{
			g = *it;
			fsalida << " & (";
			deep++;
            g->convert(fsalida);
			fsalida << ")";
			it++;
			};
	};
	
};
	
class NAnd: public And
{
	void convert(ofstream &fsalida)
	{
        fsalida << "-(";
        And::convert(fsalida);
        fsalida << ")";
	}
};

class XOr: public GateRecursive
{

	void convert(ofstream &fsalida)
	{
		
		Gate* g1 = *terms.begin();
		Gate* g2  = *(terms.begin()++);
		
		fsalida << "(";
        g1->convert(fsalida);
		fsalida << ") & -(";
        g2->convert(fsalida);
		fsalida << ") v -(";
        g1->convert(fsalida);
		fsalida << ") & (";
        g2->convert(fsalida);
		fsalida << ")"; 

	};

};
class Not: public Gate
{
	private:
	Gate* g;
	
	public:
	Not(Gate* g)
		{
			this->g=g;
			};

	void convert(ofstream &fsalida)
	{
		if (pisados.find(this)==pisados.end())
		{
			cout << "pisados: " << pisados.size() << "/" << mapa.size() << endl;
			pisados.insert(this);
			} 
			
		fsalida << "-(";
		deep++;
        g->convert(fsalida);
		deep--;
		fsalida << ")";
	};

};

int main(int argc, char* argv[])
{
	string input_filename(argv[1]);
	string output_path(argv[2]);

	ifstream input_file(input_filename.c_str());
	
	string line;
	list<string> outputs;
	int coutputs=0;
    // Process the lines
	while (getline(input_file, line))
	{
        // remove spaces from the line
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line[0] == 'I') // is Input specification
		{
			int i = line.find_first_of('(');
			string id = line.substr(i + 1, line.length() - i - 2); // get id

			Gate* g = new Input(id);
			mapa.insert(pair<string,Gate*>(id,g));
			} 
			
		if (line[0] == 'O') // es output
		{
			int p = line.find_first_of('(') + 1;
			int j = line.find_first_of(')');
			string idGate = line.substr(p, j - p);
			cout << "OUTPUT:" << idGate << endl;
			outputs.push_back(idGate);
			coutputs++;
			} else
		//es gate
		{
			int p = line.find_first_of('=');
			string idGate = line.substr(0, p);
			p=p+1;
			int j = line.find_first_of('(');
			string gate = line.substr(p, j - (p));
			p=j+1;
			j= line.length() - 1;
			
			string recorte = line.substr(p, j - p);
			list<string> tokens = split(recorte,",");
			
			list<string>::iterator it = tokens.begin();
			Gate* g;

			if (gate=="BUFF")
            {
				string a = *it;
				map<string,Gate*>::iterator s = mapa.find(*it);
				g = s->second;
				};
			if (gate=="AND")
			{
				And* a = new And();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
					};
			};
				
			if (gate=="NAND")
			{
				NAnd* a = new NAnd();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
					};
				
			};
			
			if (gate=="NOR")
			{
				NOr* a = new NOr();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
				};
			};	
			
			if (gate=="OR")
			{
				Or* a = new Or();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
				};
			};
			
			if (gate=="XOR")
			{
				XOr* a = new XOr();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
				};
			};
			
			if (gate=="NOT")
			{
				map<string,Gate*>::iterator s = mapa.find(*it);
				Gate* gh = s->second;
				g = new Not(gh);
			};
			mapa.insert(pair<string,Gate*>(idGate,g));
			}
		};
		
	
	list<string>::iterator itouts = outputs.begin();
	while (itouts!=outputs.end())
	{
		string out = *itouts;
		
		//crear archivo y escribir formula de output_path (invocacion a convert)
		ofstream output_file(output_path.c_str() + out);
			
			map<string,Gate*>::iterator s = mapa.find(out);
			Gate* g = s->second;
        g->convert(output_file);
		output_file << "." << endl;
		output_file.close();
		itouts++;
		}; 
}
