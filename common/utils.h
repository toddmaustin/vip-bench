#ifndef UTILS_H
#define UTILS_H

void mysrand(unsigned int seed);
unsigned int myrand(void);

#include<iostream>
#include<chrono>
#include<string>

    class Stopwatch
    {
        public:
            static double timeTaken;
            static bool precision;
            static int numIter;
            Stopwatch(std::string timer_name):name_(timer_name), start_time_(std::chrono::high_resolution_clock::now())
            {}
            ~Stopwatch()
            {
                auto end_time=std::chrono::high_resolution_clock::now();
                auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
                auto duration_millis=std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_);
                if(precision)
                    timeTaken=duration.count();
                else
                    timeTaken=duration_millis.count();
                
                std::cerr<<name_<<": "<<timeTaken/numIter<<(precision?" microseconds":" milliseconds")<<std::endl;
            }
        private:
            std::string name_;
            std::chrono::high_resolution_clock::time_point start_time_;
    };


#endif /* UTILS_H */
