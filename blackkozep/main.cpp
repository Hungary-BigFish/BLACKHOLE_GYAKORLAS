#include "../library/summation.hpp"
#include "../library/maxsearch.hpp"
#include "../library/stringstreamenumerator.hpp"
#include "../library/enumerator.hpp"
#include "../library/seqinfileenumerator.hpp"
#include <iostream>


using namespace std;

struct Date {
	int y;
	int m;
	int d;
	Date( int _y = 0, int _m = 0, int _d = 0 ) : y(_y), m(_m), d(_d) {}
	friend bool operator<( const Date& b, const Date& a ) {
		if ( b.y == a.y ) {
			return (b.m == a.m)?(b.d < a.d):(b.m < a.m);
		}
		else { return b.y < a.y; }
	}
	friend istream& operator>>( istream& is, Date& a ) {
		char dot;
		is >> a.y >> dot >> a.m >> dot >> a.d;
		return is;
	}
};

struct Mesurement {
	Date dt;
	int mass;
	int distance;
	Mesurement( Date _date, int _mass, int _distance ) : dt(_date), mass(_mass), distance(_distance){}
	Mesurement() {
		Date d(0,0,0);
		dt = d;
		mass = 0;
		distance = 0;
	}
	friend istream& operator >>( istream& is, Mesurement &a ) {
		is >> a.dt >> a.mass >> a.distance;
		return is;
	}
};

class MesurementSum : public Summation<Mesurement, pair<Mesurement, bool>> {
	pair<Mesurement, bool> func(const Mesurement& e) const override {
		return make_pair(e, e.distance < 3);
	}
	pair<Mesurement, bool> neutral() const override {
		Mesurement ret;
		return make_pair(ret, false);
	}
	pair<Mesurement, bool> add( const pair<Mesurement, bool>& a, const pair<Mesurement, bool>& b) const override {
		return make_pair(b.first/*rendezve van így a jobb oldalon van arégebbi mérés*/, a.second || b.second );
	}
};

struct BlackHole{
    string id;
    Mesurement m;
    bool l=false;

    friend istream& operator >>( istream& is,  BlackHole &a ) {
		//is >> a.id >> a.m;
		is >> a.id;
		string line;
		getline( is, line );
		stringstream ss( line );

		StringStreamEnumerator<Mesurement> sse = { ss };
		MesurementSum ms;
		ms.addEnumerator( &sse );
		ms.run();
		a.m = ms.result().first;
		a.l = ms.result().second;
		return is;
	}

};


class BlackHoleEnum : public Enumerator<BlackHole> {
public:
	SeqInFileEnumerator<BlackHole> *x;
	BlackHole curr;
	bool the_end;
public:
	BlackHoleEnum( const string &fn ) {
		x = new SeqInFileEnumerator<BlackHole>( fn );
	}
	~BlackHoleEnum(){
		delete x;
	}
	void first() override {
		x->first();
	}

	void next() override {
		if ( x -> end() ) {
			the_end = true;
		}
		else {

			the_end = false;
			curr = x->current();
            x->next();

		}
	}
	bool end() const override {
		return the_end;
	}
	BlackHole current() const override {
		return curr;
	}
};

class Final : public MaxSearch<BlackHole, double> {
	double func(const BlackHole& e) const override {
		return e.m.mass;
	}
	bool cond(const BlackHole& e) const override {
		return e.l;
	}
};


void safe_run( const string &file_name ) {
	try{
		Final f;
		BlackHoleEnum  bhe(file_name);
		f.addEnumerator( &bhe );
		f.run();
		cout << f.optElem().id << " " << f.opt() << endl;
	}
	catch ( SeqInFileEnumerator<BlackHole>::Exceptions &e ) {
		cout << "Error: File not found" << endl;
	}
}



int main(){
    /*BlackHole d;
    //cout << d.y;
    //Blackhole m;
    ifstream x;
    x.open("inp.txt");
    x >> d;
   // cout << d.id;*/


    /*ifstream is;
    is.open("inp.txt");
    BlackHole m;
    /*is >> m.id;
		string line;
		getline( is, line );
		stringstream ss( line );
		StringStreamEnumerator<Mesurement> sse = { ss };
		MesurementSum ms;
		ms.addEnumerator( &sse );
		ms.run();
		m.m = ms.result().first;
		m.l = ms.result().second;
        //is >> m;
        //is >> m;
		cout << m.l << " " << m.m.dt.y;*/

   /* BlackHoleEnum  bhe("inp.txt");
    bhe.first();
    bhe.next();
    cout << bhe.end() << endl;
    cout << bhe.curr.id;*/


    safe_run("inp.txt");

return 0;
}

