// test_graph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define PI 3.14159265

using namespace std;
const int N = 10000;
bool e_info[9000000];

// variables to be used 
// in both functions 
vector<int> graph[N];
vector<int> cycles[N];
double vertices_xy[1000];

// Function to mark the vertex with 
// different colors for different cycles 
void dfs_cycle(int u, int p, int color[],
	int mark[], int par[], int& cyclenumber)
{

	// already (completely) visited vertex. 
	if (color[u] == 2) {
		return;
	}

	// seen vertex, but was not completely visited -> cycle detected. 
	// backtrack based on parents to find the complete cycle. 
	if (color[u] == 1) {

		cyclenumber++;
		int cur = p;
		cout << "cur" << cur << "cyclenum: " << cyclenumber << endl;
		mark[cur] = cyclenumber;

		// backtrack the vertex which are 
		// in the current cycle thats found 
		while (cur != u) {
			cur = par[cur];
			cout << "cur" << cur << "cyclenum: " << cyclenumber << endl;
			mark[cur] = cyclenumber;
		}
		return;
	}
	par[u] = p;

	// partially visited. 
	color[u] = 1;

	// simple dfs on graph 
	for (int v : graph[u]) {

		// if it has not been visited previously 
		if (v == par[u]) {
			continue;
		}
		dfs_cycle(v, u, color, mark, par, cyclenumber);
	}

	// completely visited. 
	color[u] = 2;
}

// add the edges to the graph 
void addEdge(vector<int> adj[], int u, int v)
{
	adj[u].push_back(v);
	adj[v].push_back(u);
}

// A utility function to print the adjacency list 
// representation of graph 
void printGraph(vector<int> adj[], int V)
{
	for (int v = 0; v < V; ++v)
	{
		cout << "\n Adjacency list of vertex "
			<< v << "\n head ";
		for (auto x : adj[v])
			cout << "-> " << x;
		printf("\n");
	}
}

//returns true if the point v3 lies left of line (v1,v2)
bool isLeft(int v1, int v2, int v3, double v_xy[]) {
	double angle = 0, x[3], y[3], a[2], b[2], a1 = 0, b1 = 0;
	x[0] = v_xy[v1 * 2], y[0] = v_xy[v1 * 2 + 1];
	x[1] = v_xy[v2 * 2], y[1] = v_xy[v2 * 2 + 1];
	x[2] = v_xy[v3 * 2], y[2] = v_xy[v3 * 2 + 1];
	return ((x[1] - x[0]) * (y[2] - y[0]) - (y[1] - y[0]) * (x[2] - x[0])) > 0;
}

double get_angle(int v1, int v2, int v3, double v_xy[10]) {
	double angle = 0, x[3], y[3], a[2], b[2], a1 = 0, b1 = 0;
	x[0] = v_xy[v1 * 2], y[0] = v_xy[v1 * 2 + 1];
	x[1] = v_xy[v2 * 2], y[1] = v_xy[v2 * 2 + 1];
	x[2] = v_xy[v3 * 2], y[2] = v_xy[v3 * 2 + 1];
	//cout << "x[0], y[0], x[1], y[1], x[2], y[2]" << x[0]<< y[0]<< x[1]<< y[1]<< x[2] << y[2] << endl;
	a[0] = (x[1] - x[0]), a[1] = (y[1] - y[0]);
	b[0] = (x[1] - x[2]), b[1] = (y[1] - y[2]);
	a1 = sqrt(a[0] * a[0] + a[1] * a[1]);
	b1 = sqrt(b[0] * b[0] + b[1] * b[1]);
	angle = acosf((a[0] * b[0] + a[1] * b[1]) / (a1 * b1)) * 180.0 / PI;
	if (isLeft(v1, v2, v3, v_xy)) {
		angle = 360 - angle;
	}
	
	return angle;
}

double cost_calc(double h, double w, double l, double D2s, double D2c, int N) {
	double cost = 0, x1, x2, x3;
	x1 = powf((h + l) * (w + l), (D2c / 2));
	x2 = l * (D2c / 2) * (powf((h + l) * (w + l), (D2c / 2) - 1) * (h + w + 2 * l));
	x3 = powf(h * w, (D2s / 2));
	//cout << "x1 = " << x1 << ",  x2 = " << x2 << ", x3 = " << x3 << endl;
	cost = (N - 1) * (x1 + x2) - N * x3;
	//cost = (N - 1) * ((powf((h + l) * (w + l), (D2c / 2))) + l * (D2c / 2) * (powf((h + l) * (w + l), (D2c / 2) - 1) * (h + w + 2 * l))) - N * powf(h * w, (D2s / 2));
	//cost = (N - 1) * (powf((h * w ), (D2c / 2)))  - N * powf(h * w, (D2s / 2));
	return cost;
}

//cantor pairing function that generates a unique id
int get_id(int prev, int curr) {
	return (prev + curr) * (prev + curr + 1) / 2 + prev;
}

bool visited(int prev, int next) {
	bool vis = false;
	if (e_info[get_id(prev, next)] == 1 )		//|| e_info[next * 10 + prev] == 1)
		vis = true;
	return vis;
}

int get_current(int prev, vector<int> adj[]) {
	int curr = -1;
	auto n = adj[prev];
	for (int i = 0; i < n.size(); i++)
	{
		//cout << "n[i]: " << n[i] << endl;
		if (!(visited(prev, n[i]) || visited(n[i],prev))) {
			curr = n[i];
			break;
		}	
	}
	return curr;
}

int possible_path(int v, vector<int> adj[]) {
	int path = -1;
	auto n = adj[v];
	for (int i = 0; i < n.size(); i++)
	{
		//cout << "n[i]: " << n[i] << endl;
		if (!(visited(v, n[i]))) {
			path = n[i];
			break;
		}
	}
	return path;
}

int depair(int z) {
	long t = (int)(floor((sqrt(8 * z + 1) - 1) / 2));
	long x = t * (t + 3) / 2 - z;
	return (int) x;
}


// Driver Code 
int main()
{
	const int V = 5000;
	vector<int> adj[V];
	double v_xy[5000] = { 4,3, 6,9,10,6,9,1,14,10,15,7, 16,4, 13, 1 };
	int start, prev, curr, next, hop = 0, e_id, uc_count, edge_count = 11, max[2], min[2];
	int ucomm_arr[5000], comm_arr[5000], ucomm_id = -1, comm_id = -1, ucount = 0;
	fstream fin, fin1, fout;
	fin.open("C:/Users/nrai/Desktop/Data topk/cali_node_norm.txt", ios::in);
	fin1.open("C:/Users/nrai/Desktop/Data topk/Cali_Edge_info.txt", ios::in);
	//fout.open("C:/Users/nrai/Desktop/cali_node_norm.txt", ios::out);
	string line, temp, word;
	int c1 = 0;
	fout.precision(10);
	while (fin >> temp)
	{
		double* traj = new double[10];
		int c = 0;
		getline(fin, line);
		stringstream s(line);
		while (getline(s, word, '\t')) {
			if (word == "")
				continue;
			//fout << word;
			traj[c++] = atof(word.c_str());
			//cout << word << endl;
		}
		//fout << endl;
		//traj[1] = 10000 * (traj[1] + 124.198723) / (-119.991257 + 124.198723) ;
		//traj[2] = 10000 * (traj[2] - 40.64484) / (42.01723 - 40.64484) ;
		v_xy[(int)traj[0] * 2] = traj[1];
		v_xy[(int)traj[0] * 2 + 1] = traj[2];
		//addEdge(adj, traj[1], traj[2]);
		//fout << 0 << "\t" << c1++ << "\t" << traj[1] << "\t" << traj[2] << endl;
		printf("id: %d, X: %f, Y: %f \n", (int) traj[0], traj[1], traj[2]);
		delete traj;
	}
	
	while (fin1 >> temp)
	{
		double* traj1 = new double[10];
		int c1 = 0;
		getline(fin1, line);
		stringstream s(line);
		while (getline(s, word, ' ')) {
			if (word == "")
				continue;
			//fout << word;
			traj1[c1++] = atof(word.c_str());
			//cout << word << endl;
		}
		//fout << endl;
		//addEdge(adj, (int) traj1[0], (int) traj1[1]);
		//traj[1] = 10000 * (traj[1] + 124.198723) / (-119.991257 + 124.198723);
		//traj[2] = 10000 * (traj[2] - 40.64484) / (42.01723 - 40.64484);
		//v_xy[(int)traj[2] * 2] = traj[0];
		//v_xy[(int)traj[2] * 2 + 1] = traj[1];
		addEdge(adj, (int) traj1[0], (int) traj1[1]);
		//fout << c1++ << "\t" << traj[1] << "\t" << traj[2] << endl;
		//printf("start: %d, end: %d, length: %f \n", (int) traj1[0], (int) traj1[1], traj1[2]);
		delete traj1;
	}

	fout.close();

	int n = 100;
	printf("x: %lf, y: %lf\n", v_xy[n * 2], v_xy[n * 2 + 1]);
	
	//printGraph(adj, 2000);
	//vector<int> adj[V];

	/*addEdge(adj, 0, 1);
	addEdge(adj, 0, 3);
	addEdge(adj, 1, 2);
	addEdge(adj, 1, 4);
	addEdge(adj, 2, 3);
	addEdge(adj, 2, 5);
	addEdge(adj, 5, 3);
	addEdge(adj, 4, 5);
	addEdge(adj, 5, 6);
	addEdge(adj, 6, 7);
	addEdge(adj, 3, 7);
	//printGraph(adj, V);
	//cout << "vertex 5: " << v_xy[5 * 2 - 1] << "," << v_xy[5 * 2] << endl;
	//cout << get_angle(0, 1, 2, v_xy) << endl;
	//cout << acosf(1/sqrt(2)) * 180.0 / PI << endl;

	start = 0;
	*/
	for (int j = 0; j < V; j++) {
		start = j;
		//cout << "start: " << start << endl;
		while (possible_path(start, adj) != -1) {
			max[0] = max[1] = 0;
			min[0] = min[1] = 100;
			int temp_e[10];
			hop = 0;
			prev = start;
			next = -1;
			if (possible_path(prev, adj) == -1) {
				//cout << "prev= " << prev << endl;
				//cout << "no next vertex" << endl;
				continue;
			}
			else {
				curr = possible_path(prev, adj);
				//cout << "heres current: " << curr << endl;
			}
			cout << "visited: " << prev << "," << curr << endl;
			e_id = get_id(prev, curr);
			cout << "added: " << e_id << " at "<< hop << endl;
			e_info[e_id] = 1;
			temp_e[hop] = e_id;
			//cout << "eid = " << get_id(prev, curr) << endl;
			auto n1 = adj[curr];

			while (next != start){ //&& hop <= 6) {
				double min_ang = 360;
				for (int i = 0; i < n1.size(); i++) {
					double ang = 360;
					//cout << "-> " << n1[i] << endl;
					//cout << "prev= " << prev << ", curr= " << curr << ", next = " << n1[i] << endl;
					if (visited(curr, n1[i])) {
						ang = 361;
						//cout << "already visited.." << endl;
					}
					else
						ang = get_angle(prev, curr, n1[i], v_xy);
					//cout << ang << endl;
					if (ang < min_ang && ang != 0) {
						min_ang = ang;
						next = n1[i];
					}

				}

				if (min_ang > 180) {
					cout << "no right vertex to go" << endl;
					hop = 0;
					//cout << "prev= " << prev << ", curr= " << curr << ", next = " << next << endl;
					//e_info[get_id(prev, curr)] = 0;
					cout << min_ang << endl;
					break;
				}
				prev = curr;
				curr = next;
				//cout << "prev= " << prev << ", curr= " << curr << ", next = " << next << endl;
				e_id = get_id(prev, curr);
				cout << "visited: " << prev << "," << curr << endl;
				e_info[e_id] = 1;
				temp_e[++hop] = e_id;
				cout << "added: " << e_id << " at " << hop << endl;
				//cout << "eid = " << e_id << endl;
				//cout << next << endl;
				n1 = adj[curr];
				//hop++;
			}
			cout << "hops = " << hop << endl;
			if (hop > 1) {
				ucomm_id++;
				ucomm_arr[ucount++] = ucomm_id;
				ucomm_arr[ucount++] = hop + 1;
				cout << "vertices: " << hop + 1 << endl;
				for (int k = 0; k < hop+1; k++)
				{
					int v_id = depair(temp_e[k]);
					int tx = v_xy[v_id*2], ty = v_xy[v_id*2+1];
					cout << tx << ", " << ty << endl;
					if (tx > max[0])
						max[0] = tx;
					if (ty > max[1])
						max[1] = ty;
					if (tx < min[0])
						min[0] = tx;
					if (ty < min[1])
						min[1] = ty;
					cout << depair(temp_e[k]) << endl;
					ucomm_arr[ucount++] = temp_e[k];
					
					cout << depair(temp_e[k]) << "x: " << v_xy[depair(temp_e[k])*2] << ", y: "<< v_xy[2*depair(temp_e[k])+1] << " sss ";
				}
				cout << "minx: " << min[0] << ", miny: " << min[1] << "maxx: " << max[0] << ", maxy: " << max[1];
				cout << endl;
			}
			

		}
	}
	
	for (int i = 0; i < ucount; i++)
	{
		cout << ucomm_arr[i] << endl;
	}
	cout << "val: " << depair(39) << endl;
	/*min_ang = 360;
		for (int i = 0; i < n1.size(); i++) {
			double ang;
			cout << "-> " << n1[i] << endl;
			cout << "prev= " << prev << ", curr= " << curr << ", next = " << n1[i] << endl;
			ang = get_angle(prev, curr, n1[i], v_xy);
			cout << ang << endl;
			if (ang < min_ang && ang != 0) {
				min_ang = ang;
				next = n1[i];
			}
		}
		cout << next << endl;
		prev = curr;
		curr = next;
		cout << "prev= " << prev << ", curr= " << curr << ", next = " << next << endl;
		cout << next << endl;
		n1 = adj[curr];
		min_ang = 360;
		for (int i = 0; i < n1.size(); i++) {
			double ang;
			cout << "-> " << n1[i] << endl;
			cout << "prev= " << prev << ", curr= " << curr << ", next = " << n1[i] << endl;
			ang = get_angle(prev, curr, n1[i], v_xy);
			cout << ang << endl;
			if (ang < min_ang && ang != 0) {
				min_ang = ang;
				next = n1[i];
			}
		}
		cout << next << endl;
	}*/
	fin.close();
	//delete adj;
	return 0;
}
 