#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct Pair
{
	double x;
	double y;
	bool operator==(Pair p) {
		return p.x == this->x && p.y == this->y;
	}
	Pair(double x, double y) {
		this->x = x;
		this->y = y;
	}
	Pair() {};
};

//actrually this structer is stupid, Pair is enough to complete the work. 
//my initial goal is store the Cluster of every point in the point, this is stupid. However, i'm too lazy to rewrite those functions
//maybe i'll optimize it someday, i'm a little bit buzy this week
struct Point
{
	Pair SelfPosition;
	Pair Cluster;
	Point(double x, double y) {
		SelfPosition = Pair(x, y);
		Cluster = Pair(x, y);
	}
	Point(const Point& p) {
		this->SelfPosition = Pair(p.SelfPosition.x,p.SelfPosition.y);
		this->Cluster = Pair(p.Cluster.x, p.Cluster.y);
	};
};

ostream& operator<<(ostream& os, const Point& p) {
	os << "(" << p.SelfPosition.x << "," << p.SelfPosition.y << ")";
	return os;
}

//not calculating root distance, root operation is useless 
double SquareDistance(const Point& p1, const Point& p2) {
	return (p1.SelfPosition.x - p2.SelfPosition.x) * (p1.SelfPosition.x - p2.SelfPosition.x)\
		+ (p1.SelfPosition.y - p2.SelfPosition.y) * (p1.SelfPosition.y - p2.SelfPosition.y);
}

//calculate the nearest cluster centroid of a point
int GetCluster(const Point& p, const vector<Point>& ClusterCentroids) {
	double min = 999999999;
	int Group = -1;
	for (int i = 0; i < ClusterCentroids.size(); i++) {
		int t = SquareDistance(p, ClusterCentroids[i]);
		if (t < min) {
			min = t;
			Group = i;
		}
	}
	return Group;
}

//update the cluster centroids to the mean value of the points in the group, and find out if those cluster centroids are suitable
bool UpDateClusterCenter(const vector<vector<Point>>& Groups, vector<Point>& ClusterCentroids) {
	bool IfChange = false;
	double x_sum = 0, y_sum = 0;
	for (int i = 0; i < Groups.size(); i++) {
		x_sum = 0;
		y_sum = 0;
		for (int j = 0; j < Groups[i].size(); j++) {
			x_sum += Groups[i][j].SelfPosition.x;
			y_sum += Groups[i][j].SelfPosition.y;
		}
		//calculate the mean value
		x_sum /= Groups[i].size();
		y_sum /= Groups[i].size();
		Pair temp(x_sum, y_sum);
		//judge if the cluster centroids change
		if (ClusterCentroids[i].SelfPosition == temp) {
			continue;
		}
		else {
			ClusterCentroids[i].SelfPosition = temp;
			IfChange = true;
		}
		
	}
	return IfChange;
}

//K_means algorithmn
vector<Point> K_means(int k,const vector<Point>& p) {
	vector<vector<Point>> Groups;
	vector<Point> ClusterCentroids;
	for (int i = 0; i < k; i++) {
		ClusterCentroids.push_back(Point(p[i]));
		vector<Point> t;
		Groups.push_back(t);
	}
	do {
		for (int i = 0; i < p.size(); i++) {
			Groups[GetCluster(p[i], ClusterCentroids)].push_back(p[i]);
		}
	} while (UpDateClusterCenter(Groups, ClusterCentroids));
	return ClusterCentroids;
}

int main() {
	//--------------test case 1: correct----------------
	//Point p1(0, 0);
	//Point p2(1, 1);
	//vector<Point> p;
	//p.push_back(p1);
	//p.push_back(p2);
	//--------------------------------------------------

	Point p1(1, 6);
	Point p2(8, 3);
	Point p3(7, 1);
	Point p4(9, 8);
	Point p5(1, 2);
	Point p6(9, 3);
	Point p7(5, 6);
	vector<Point> p = {p1, p2, p3, p4, p5, p6, p7};
	//get our cluster centroids
	vector<Point> ClusterCentroids = K_means(3, p);
	for (int i = 0; i < ClusterCentroids.size(); i++) {
		cout << ClusterCentroids[i] << endl;
	}
}