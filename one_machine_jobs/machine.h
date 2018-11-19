#pragma once
#include <vector>

struct job
{
	int index;
	int weight;
	int processing_time;
	int due_date;
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
	void print_jobs();

	void init_tree(const job_type &type);

private:
	int index_ = 0;
	int optimum_ = INT_MAX;
	std::vector<job> optimal_job_{};
	std::vector<job> jobs_{};


	void build_tree(std::vector<int> numbers, std::vector<job> permutations, const job_type &type);


};

