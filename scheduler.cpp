#include <iostream>
#include <string>
#include "scheduler.h"
#include <sstream>
#include <iomanip>
#include <math.h>
#include <algorithm>

Scheduler Scheduler::_scheduler;

Scheduler::Scheduler()
{
    Scheduler::_scheduler.isTrace = false;
}

void Scheduler::parse()
{
    std::string line;

    // Line 1 - > trace or stats
    std::getline(std::cin, line);
    if (line == "trace")
        Scheduler::_scheduler.isTrace = true;

    // Line 2 -> required algorithms
    std::getline(std::cin, line);
    std::istringstream ss(line);
    std::string token;

    // loop to extract tokens separated by commas

    while (std::getline(ss, token, ','))
    {
        std::vector<int> algorithm;
        algorithm.resize(2);

        // getting pos of hyhpen in current token
        size_t hyphenPos = token.find('-');

        // checks if hyphen pos is valid (there is hyphen)
        if (hyphenPos != std::string::npos)
        {
            std::istringstream(token.substr(0, hyphenPos)) >> algorithm[0];
            std::istringstream(token.substr(hyphenPos + 1)) >> algorithm[1];
        }
        else
        {
            algorithm[0] = std::stoi(token);
        }

        _scheduler.algorithms.push_back(algorithm);
    }

    // Line 3 -> last instant
    std::getline(std::cin, line);
    _scheduler.lastInstant = std::stoi(line);

    // Line 4 -> Num Processes
    std::getline(std::cin, line);
    _scheduler.numProcesses = std::stoi(line);

    // Line 5 -> Processes
    _scheduler.processess = (Process **)malloc(numProcesses * sizeof(Process *));

    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        _scheduler.processess[i] = new Process;

        std::getline(std::cin, line);

        size_t commaPos = line.find(','); // first comma pos

        _scheduler.processess[i]->order = i;

        std::istringstream(line.substr(0, commaPos)) >> _scheduler.processess[i]->name;

        line = line.substr(commaPos + 1); // removes until after first comma
        commaPos = line.find(',');        // find next comma
        std::istringstream(line.substr(0, commaPos)) >> _scheduler.processess[i]->arrivalTime;

        std::istringstream(line.substr(commaPos + 1)) >> _scheduler.processess[i]->serviceTime;

        // _scheduler.processess[i]->serviceTime = _scheduler.processess[i]->serviceTime;
        _scheduler.processess[i]->remainingTime = _scheduler.processess[i]->serviceTime;
    }
    _scheduler.board.resize(_scheduler.numProcesses, std::vector<char>(_scheduler.lastInstant, ' '));
    _scheduler.processessVector.resize(_scheduler.numProcesses);
    _scheduler.finishes.resize(_scheduler.numProcesses);
    _scheduler.turnarounds.resize(_scheduler.numProcesses);
    _scheduler.normturns.resize(_scheduler.numProcesses);
}

void Scheduler::printInfo()
{
    std::cout << "Trace: " << _scheduler.isTrace << std::endl;

    for (int i = 0; i < algorithms.size(); i++)
    {
        std::cout << "Algorithm: " << algorithms[i][0];
        std::cout << ", Parameter: " << algorithms[i][1];
    }

    std::cout << "Last Instant: " << _scheduler.lastInstant << std::endl;
    std::cout << "Number of Processes: " << _scheduler.numProcesses << std::endl;

    for (int i = 0; i < numProcesses; i++)
    {
        Process *process = _scheduler.processess[i];

        std::cout << "Process Name: " << process->name;
        std::cout << ", Arrival: " << process->arrivalTime;
        std::cout << ", Service: " << process->serviceTime;
        std::cout << std::endl;
    }
}
void Scheduler::print_trace(std::string algoname)
{

    std::cout << algoname;

    if (algoname == "SPN" || algoname == "SRT")
    {
        std::cout << "   ";
        algoname.resize(algoname.size() + 1);
    }

    else if (algoname == "Aging" || algoname == "FB-2i")
    {
        std::cout << " ";
        algoname.resize(algoname.size() - 1);
    }

    else
        std::cout << "  ";

    for (int i = 0; i <= _scheduler.lastInstant; i++)
    {
        std::cout << i % 10 << " ";
    }

    int i = 0;
    std::cout << "\n";
    while (i < (4 + 2 + (2 * _scheduler.lastInstant)))
    {
        std::cout << "-";
        i++;
    }
    std::cout << "--";
    std::cout << "\n";
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        std::cout << _scheduler.processessVector[i]->name << "     ";
        for (int j = 0; j <= _scheduler.lastInstant; j++)
        {

            if (_scheduler.board[i][j] == '.' || _scheduler.board[i][j] == '*')
                std::cout << '|' << _scheduler.board[i][j];

            else
                std::cout << '|' << " ";
        }
        std::cout << "\n";
    }

    i = 0;
    while (i < (algoname.length() + 2 + (2 * _scheduler.lastInstant)))
    {
        std::cout << "-";
        i++;
    }
    std::cout << "--";
    std::cout << "\n\n";
}

void Scheduler::print_stats(std::string algoname)
{
    std::cout << algoname << "\n";
    std::cout << "Process" << "    " << "|";
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        std::cout << "  " << static_cast<char>('A' + i) << "  " << "|";
    }

    std::cout << "\n"
              << "Arrival" << "    " << "|";
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        if (_scheduler.processess[i]->arrivalTime <= 9)
            std::cout << "  ";

        else
            std::cout << " ";

        std::cout << _scheduler.processess[i]->arrivalTime << "  " << "|";
    }

    std::cout << "\n"
              << "Service" << "    " << "|";
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        if (_scheduler.processess[i]->serviceTime <= 9)
            std::cout << "  ";

        else
            std::cout << " ";

        std::cout << _scheduler.processess[i]->serviceTime << "  " << "|";
    }

    std::cout << " Mean" << "|" << "\n";

    // std::vector<int>finishes(_scheduler.numProcesses);
    // finishes[0]=(_scheduler.processess[0]->arrivalTime + _scheduler.processessVector[0]->serviceTime);

    std::cout << "Finish" << "     " << "|";
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        // finishes[i]=finishes[i-1]+_scheduler.processessVector[i]->serviceTime;
        if (finishes[i] <= 9)
            std::cout << "  ";

        else
            std::cout << " ";

        std::cout << finishes[i] << "  " << "|";
    }
    std::cout << "-----|\n";

    double sum = 0;

    std::cout << "Turnaround" << " " << "|";
    // std::vector<double>turnarounds(_scheduler.numProcesses);
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        turnarounds[i] = finishes[i] - _scheduler.processess[i]->arrivalTime;
        int x = turnarounds[i];
        sum += x;

        if (x <= 9)
            std::cout << "  ";

        else
            std::cout << " ";

        std::cout << x << "  " << "|";
    }

    double x = sum / numProcesses;

    if (x <= 9)
        std::cout << " ";

    std::cout << std::fixed << std::setprecision(2) << (x) << "|\n";

    std::cout << "NormTurn" << "   " << "|";
    // std::vector<double>normturns(_scheduler.numProcesses);
    sum = 0;
    for (int i = 0; i < _scheduler.numProcesses; i++)
    {
        normturns[i] = turnarounds[i] / _scheduler.processess[i]->serviceTime;
        sum += normturns[i];

        if (normturns[i] <= 9)
            std::cout << " ";

        else
            std::cout << "";

        std::cout << normturns[i] << "|";
    }
    std::cout << " " << std::fixed << std::setprecision(2) << (sum / numProcesses) << "|\n\n";
}

void Scheduler::firstComeFirstServe()
{
    if (_scheduler.isTrace)
    {
        int t = 0, p = 0;
        _scheduler.processessVector[0]->active = true;
        while (t < _scheduler.lastInstant)
        {
            for (int i = 0; i < _scheduler.numProcesses; i++)
            {
                if (_scheduler.processessVector[i]->active && _scheduler.processessVector[i]->serviceTime > 0)
                {
                    p = i;
                    _scheduler.processessVector[i]->serviceTime--;
                    if (_scheduler.processessVector[i]->serviceTime == 0)
                    {
                        p = i + 1;
                        _scheduler.processessVector[i]->active = false;
                        _scheduler.processessVector[i]->killed = true;
                    }
                    _scheduler.board[i][t] = '*';
                }
                else if ((_scheduler.processessVector[i]->arrivalTime <= t) && (!_scheduler.processessVector[i]->active && !_scheduler.processessVector[i]->killed))
                {
                    _scheduler.board[i][t] = '.';
                }

                else if (_scheduler.processessVector[i]->arrivalTime > t)
                    break;
            }
            if (p < _scheduler.numProcesses)
                _scheduler.processessVector[p]->active = true;
            t++;
        }
    }
    else
    {
        finishes[0] = (_scheduler.processess[0]->arrivalTime + _scheduler.processessVector[0]->serviceTime);

        for (int i = 1; i < _scheduler.numProcesses; i++)
            finishes[i] = finishes[i - 1] + _scheduler.processessVector[i]->serviceTime;
    }
}

void Scheduler::RoundRobin(int quantum)
{
    int t = 0, quantumTime = 0;
    int index = _scheduler.insert_ready_processes(0, 0);
    while (t < _scheduler.lastInstant)
    {
        Process *p = nullptr;
        if (!_scheduler.processessQueue.empty())
        {
            p = _scheduler.processessQueue.front();

            _scheduler.processessQueue.pop();
        }

        else
        {
            index = _scheduler.insert_ready_processes(index, t);
            t++;
        }

        while (quantumTime < quantum && p->serviceTime > 0)
        {
            p->serviceTime--;
            board[p->order][t] = '*';

            std::queue<Process *> copyQueue = _scheduler.processessQueue;
            while (!copyQueue.empty())
            {
                Process *p = copyQueue.front();
                copyQueue.pop();
                board[p->order][t] = '.';
            }

            quantumTime++, t++;
            index = _scheduler.insert_ready_processes(index, t);
        }
        if (p->serviceTime > 0)
            processessQueue.push(p);
        else
            finishes[p->order] = t;

        quantumTime = 0;
    }
}
int Scheduler::insert_ready_processes(int index, int t)
{

    while (index < _scheduler.numProcesses && Scheduler::_scheduler.processessVector[index]->arrivalTime <= t)
    {
        Scheduler::_scheduler.processessQueue.push(Scheduler::_scheduler.processessVector[index++]);
    }
    return index;
}

void Scheduler::shortest_process_next()
{
    //_scheduler.processessPrq.push(_scheduler.processessVector[0]);
    int t = 0, index = 0;

    while (t < _scheduler.lastInstant)
    {
        Process *p = nullptr;

        while (index < _scheduler.numProcesses && _scheduler.processessVector[index]->arrivalTime <= t)
        {
            Scheduler::_scheduler.processessSprq.push(Scheduler::_scheduler.processessVector[index++]);
            if (_scheduler.processessSprq.empty())
                t++;
        }

        if (_scheduler.processessSprq.empty())
            break;

        else
        {
            p = _scheduler.processessSprq.top();
            _scheduler.processessSprq.pop();
        }

        while (p->serviceTime > 0)
        {
            board[p->order][t] = '*';

            std::priority_queue<Process *, std::vector<Process *>, CompareService> copyprq = _scheduler.processessSprq;
            while (!copyprq.empty())
            {
                Process *p = copyprq.top();
                copyprq.pop();
                board[p->order][t] = '.';
            }

            t++;
            p->serviceTime--;

            while (index < _scheduler.numProcesses && _scheduler.processessVector[index]->arrivalTime <= t)
                Scheduler::_scheduler.processessSprq.push(Scheduler::_scheduler.processessVector[index++]);
        }

        finishes[p->order] = t;
    }
}

void Scheduler::shortest_remaining_time()
{
    time = 0;
    int index = 0;
    Process *activeProcess = NULL;
    std::priority_queue<Process *, std::vector<Process *>, CompareRemainingTime> minHeapRemainingTime;
    std::priority_queue<Process *, std::vector<Process *>, CompareArrivalTime> minHeapArrivalTime;
    std::vector<Process *> temp;

    while (time <= lastInstant)
    {
        // check for process arrival

        if (index < numProcesses && processessVector[index]->arrivalTime == time)
        {
            minHeapRemainingTime.push(processessVector[index]);
            index++;
        }

        // check if active process finished

        if (activeProcess && !activeProcess->remainingTime)
        {
            finishes[activeProcess->order] = time;
            activeProcess = NULL;
        }

        // check if there are waiting processes
        if (!minHeapRemainingTime.empty())
        {
            if (!activeProcess || minHeapRemainingTime.top()->remainingTime < activeProcess->remainingTime)
            {
                Process *lastActiveProcess = activeProcess;

                do
                {
                    minHeapArrivalTime.push(minHeapRemainingTime.top());
                    minHeapRemainingTime.pop();
                } while (!minHeapRemainingTime.empty() && minHeapRemainingTime.top()->remainingTime == minHeapArrivalTime.top()->remainingTime);

                activeProcess = minHeapArrivalTime.top();
                minHeapArrivalTime.pop();

                while (!minHeapArrivalTime.empty())
                {
                    minHeapRemainingTime.push(minHeapArrivalTime.top());
                    minHeapArrivalTime.pop();
                }

                if (lastActiveProcess)
                {
                    minHeapRemainingTime.push(lastActiveProcess);
                }
            }
        }

        // printing
        if (activeProcess)
        {
            board[activeProcess->order][time] = '*';
        }

        temp.clear();

        while (!minHeapRemainingTime.empty())
        {
            Process *process = minHeapRemainingTime.top();
            minHeapRemainingTime.pop();

            board[process->order][time] = '.';

            temp.push_back(process);
        }

        for (int i = 0; i < temp.size(); i++)
        {
            minHeapRemainingTime.push(temp[i]);
        }

        time++;

        if (activeProcess)
        {
            activeProcess->remainingTime--;
        }
    }

    if (activeProcess && !finishes[activeProcess->order])
    {
        finishes[activeProcess->order] = lastInstant;
    }
}

float Scheduler::response_ratio(Process *process)
{
    return ((time - process->arrivalTime) + process->serviceTime) / process->serviceTime;
}

void Scheduler::highest_response_ratio_next()
{
    time = 0;
    int index = 0;
    Process *activeProcess = NULL;
    std::vector<Process *> waiting;

    while (time <= lastInstant)
    {
        if (index < numProcesses && processessVector[index]->arrivalTime == time)
        {
            waiting.push_back(processessVector[index]);
            index++;
        }

        if (activeProcess != NULL && !activeProcess->remainingTime)
        {
            finishes[activeProcess->order] = time;
        }

        if (activeProcess == NULL || !activeProcess->remainingTime)
        {
            if (waiting.size())
            {
                int highest_ratio_index = 0;
                float highest_ratio = response_ratio(waiting[0]);
                float temp_ratio;

                for (int i = 1; i < waiting.size(); i++)
                {
                    temp_ratio = response_ratio(waiting[i]);

                    if (temp_ratio > highest_ratio)
                    {
                        highest_ratio_index = i;
                        highest_ratio = temp_ratio;
                    }
                }

                activeProcess = waiting[highest_ratio_index];

                auto it = waiting.begin() + highest_ratio_index;
                waiting.erase(it);
            }
            else
            {
                activeProcess = NULL;
            }
        }

        if (activeProcess != NULL)
        {
            board[activeProcess->order][time] = '*';
        }

        for (int i = 0; i < waiting.size(); i++)
        {
            board[waiting[i]->order][time] = '.';
        }

        time++;

        if (activeProcess != NULL)
        {
            activeProcess->remainingTime--;
        }
    }

    if (activeProcess && !finishes[activeProcess->order])
    {
        finishes[activeProcess->order] = lastInstant;
    }
}

void Scheduler::feedback(bool variableQuantum)
{
    time = 0;
    int noOfWaiting = 0;
    int currentQuantum = 0;
    int index = 0;
    Process *activeProcess = NULL;
    std::queue<Process *> baseQueue;
    std::vector<std::queue<Process *>> queues;

    queues.push_back(baseQueue);

    while (time <= lastInstant)
    {
        // check process arrival
        if (index < numProcesses && processessVector[index]->arrivalTime == time)
        {
            queues[0].push(processessVector[index]);
            processessVector[index]->lastQueueIndex = 0;
            index++;
            noOfWaiting++;
        }

        // std::cout << "Time: " << time;
        // std::cout << ", Active Process: " << (activeProcess ? activeProcess->name : "NONE") << std::endl;

        // check if process finished

        if (activeProcess && !activeProcess->remainingTime)
        {
            // std::cout << activeProcess->name << " finished" << std::endl;
            currentQuantum = 0;
            finishes[activeProcess->order] = time;
            activeProcess = NULL;
        }

        // check current quantum time

        if (!currentQuantum)
        {
            if (noOfWaiting)
            {
                Process *lastActiveProcess = activeProcess;

                for (int i = 0; i < queues.size(); i++)
                {
                    if (queues[i].size())
                    {
                        activeProcess = queues[i].front();
                        queues[i].pop();
                        noOfWaiting--;
                        break;
                    }
                }

                if (lastActiveProcess)
                {
                    lastActiveProcess->lastQueueIndex++;
                    if (!(lastActiveProcess->lastQueueIndex) < queues.size())
                    {
                        std::queue<Process *> queue;
                        queues.push_back(queue);
                    }

                    queues[lastActiveProcess->lastQueueIndex].push(lastActiveProcess);
                    noOfWaiting++;
                }
            }

            if (activeProcess)
            {
                currentQuantum = (variableQuantum) ? pow(2, activeProcess->lastQueueIndex) : 1;
            }
        }

        // printing

        if (activeProcess)
        {
            board[activeProcess->order][time] = '*';
        }

        if (noOfWaiting)
        {

            for (int i = 0; i < queues.size(); i++)
            {
                std::vector<Process *> temp;

                while (!queues[i].empty())
                {
                    Process *process = queues[i].front();

                    board[process->order][time] = '.';

                    queues[i].pop();
                    temp.push_back(process);
                }

                for (int j = 0; j < temp.size(); j++)
                {
                    queues[i].push(temp[j]);
                }

                temp.clear();
            }
        }

        time++;

        if (activeProcess)
        {
            currentQuantum--;
            activeProcess->remainingTime--;
        }
    }

    if (activeProcess && !finishes[activeProcess->order])
    {
        finishes[activeProcess->order] = lastInstant;
    }
}

void Scheduler::Aging(int quantum)
{
    int t = 0, index = 0, insertionOrdercounter = 0;
    std::priority_queue<Process *, std::vector<Process *>, CompareAging> copyprq;

    while (t < lastInstant)
    {
        // std::cout<<t<<"\n";
        while (index < numProcesses and processessVector[index]->arrivalTime <= t)
        { // std::cout<<processessVector[index]->name<<processessVector[index]->serviceTime<<"\n";
            processessVector[index]->serviceTime++;
            processessVector[index]->insertionOrder = insertionOrdercounter++;
            processessPprq.push(processessVector[index++]);
        }

        if (processessPprq.empty())
            break;

        Process *p = processessPprq.top();
        processessPprq.pop();

        int q = 0;
        while (q < quantum)
        {
            // std::cout<<p->name<<p->serviceTime<<"\n";
            board[p->order][t] = '*';

            copyprq = processessPprq;
            while (!copyprq.empty())
            {
                Process *p = copyprq.top();
                copyprq.pop();
                board[p->order][t] = '.';
                p->serviceTime++;
            }
            t++;
            q++;
        }
        copyprq = processessPprq;

        // std::cout<<t<<"\n";
        while (!copyprq.empty())
        {
            Process *p = copyprq.top();
            copyprq.pop();
            // p->serviceTime ++;
            // std::cout<<p->name<<p->serviceTime<<"\n";
        }
        p->serviceTime = processess[p->order]->serviceTime;
        // std::cout<<p->name<<p->serviceTime<<"\n";
        p->insertionOrder = insertionOrdercounter++;
        processessPprq.push(p);
    }
}

void Scheduler::compute_algorithm()
{

    for (int j = 0; j < algorithms.size(); j++)
    {
        for (int i = 0; i < _scheduler.numProcesses; i++)

            _scheduler.processessVector[i] = new Process(*_scheduler.processess[i]);

        sort(_scheduler.processessVector.begin(), _scheduler.processessVector.end(), CompareArrivalTime2());

        std::string algoname;

        int algonum = (algorithms[j])[0];
        int algoPar = (algorithms[j])[1];

        switch (algonum)
        {
        case 1:
            algoname = "FCFS";
            _scheduler.firstComeFirstServe();
            break;
        case 2:
            algoname = "RR-" + std::to_string(algoPar);
            _scheduler.RoundRobin(algoPar);
            break;
        case 3:
            algoname = "SPN";
            _scheduler.shortest_process_next();
            break;
        case 4:
            algoname = "SRT";
            _scheduler.shortest_remaining_time();
            break;
        case 5:
            algoname = "HRRN";
            _scheduler.highest_response_ratio_next();
            break;
        case 6:
            algoname = "FB-1";
            _scheduler.feedback(false);
            break;
        case 7:
            algoname = "FB-2i";
            _scheduler.feedback(true);
            break;
        case 8:
            algoname = "Aging";
            _scheduler.Aging(algoPar);
            break;
        }

        if (_scheduler.isTrace)
            print_trace(algoname);

        else
            print_stats(algoname);

        while (!_scheduler.processessQueue.empty())
            _scheduler.processessQueue.pop();

        while (!_scheduler.processessSprq.empty())
            _scheduler.processessSprq.pop();

        while (!_scheduler.processessPprq.empty())
            _scheduler.processessPprq.pop();

        for (auto &row : board)
            std::fill(row.begin(), row.end(), ' ');
    }
}

int main()
{

    Scheduler::_scheduler.parse();
    // Scheduler::_scheduler.printInfo();
    Scheduler::_scheduler.compute_algorithm();

    return 0;
}
