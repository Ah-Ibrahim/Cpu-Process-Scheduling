#ifndef scheduler_h
#define scheduler_h
#include <string>
#include <map>
#include <vector>
#include <queue>

typedef struct
{
    std::string name;
    int arrivalTime;
    int serviceTime;
    int priorityTime;
    int remainingTime;
    int finishTime;
    int turnAroundTime;
    float normTurn;
    int order;
    int insertionOrder = 0;
    bool active = false;
    bool killed = false;
    int lastQueueIndex = -1;
} Process;

struct CompareRemainingTime
{
    bool operator()(const Process *a, const Process *b)
    {
        return a->remainingTime > b->remainingTime;
    }
};

struct CompareArrivalTime
{
    bool operator()(const Process *a, const Process *b)
    {
        return a->arrivalTime > b->arrivalTime;
    }
};

struct CompareArrivalTime2
{
    bool operator()(const Process *a, const Process *b)
    {
        return a->arrivalTime < b->arrivalTime;
    }
};
struct CompareService
{
    bool operator()(const Process *a, const Process *b)
    {
        return a->serviceTime > b->serviceTime;
    }
};
struct CompareAging
{
    bool operator()(const Process *a, const Process *b)
    {
        if (a->serviceTime != b->serviceTime)
            return a->serviceTime < b->serviceTime;
        else
            return a->insertionOrder > b->insertionOrder;
    }
};

struct Scheduler
{
    bool isTrace;
    int lastInstant;
    int numProcesses;
    std::vector<std::vector<int>> algorithms;
    Process **processess;

    // for algorithms
    int time;
    std::queue<Process *> processessQueue;
    std::priority_queue<Process *, std::vector<Process *>, CompareService> processessSprq;
    std::priority_queue<Process *, std::vector<Process *>, CompareAging> processessPprq;
    std::vector<std::vector<char>> board;
    std::vector<Process *> processessVector;
    std::vector<int> finishes;
    std::vector<double> turnarounds;
    std::vector<double> normturns;

    Scheduler();

    // Main
    void printInfo();
    void print_trace(std::string s);
    void print_stats(std::string s);
    void parse();
    void compute_algorithm();

    // Implemented Algorithms
    void firstComeFirstServe();
    void RoundRobin(int x);
    void shortest_process_next();
    void shortest_remaining_time();
    void highest_response_ratio_next();
    void feedback(bool variableQuantum);
    void Aging(int x);

    // Helping algorithms
    int insert_ready_processes(int x, int y);
    float response_ratio(Process *process);

    static Scheduler _scheduler;
};

#endif