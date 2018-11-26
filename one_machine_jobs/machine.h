#pragma once
#include <vector>

struct job
{
	int index;
	int weight;
	int processing_time;
	int due_date;
	double wpt;
};

class result
{
public:
	int optimum;
	std::vector<int> res_order;

	bool operator==(result &r1) 
	{
		return optimum == r1.optimum && res_order == r1.res_order;
	}
};

class machine
{
public:
	machine() = default;
	~machine() = default;

	enum job_type
	{
		lateness = 0,
		tardiness = 1
	};

	void add_job(int w, int p, int d);
	void erase_jobs();
	void print_jobs();

	result init_tree(const job_type &type);

	result calculate_order();

	

private:
	int index_ = 0;
	int optimum_ = INT_MAX;
	std::vector<job> optimal_job_{};
	std::vector<job> jobs_{};
	static bool sort_by_wpt(job j1, job j2);


	void build_tree(std::vector<int> numbers, std::vector<job> permutations, const job_type &type);


};

