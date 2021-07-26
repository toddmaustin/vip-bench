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
      static int nSlots;
      static bool tableFormat;
      Stopwatch(std::string timer_name):name_(timer_name), start_time_(std::chrono::high_resolution_clock::now())
      {
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        cycles=((uint64_t)hi << 32) | lo;
      }
      ~Stopwatch()
      {
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        cycles=(((uint64_t)hi << 32) | lo) - cycles;
        auto end_time=std::chrono::high_resolution_clock::now();
        auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
        auto duration_millis=std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_);
        if(precision)
          timeTaken=duration.count();
        else
          timeTaken=duration_millis.count();
        if(!tableFormat)
        std::cerr<<name_<<" : "<<timeTaken/numIter<<(precision?" microseconds, ":" milliseconds, ")<<cycles<<" processor cycles"<<std::endl;
        else
        std::cerr<<name_<<" : "<</*timeTaken/numIter*/duration_millis.count()<<"\t"<<(timeTaken/numIter)/nSlots<<"\t"<<cycles<<std::endl;
      }
    private: 
      std::string name_;
      std::chrono::high_resolution_clock::time_point start_time_;
      uint64_t cycles;
      unsigned int lo,hi;
  };


#ifdef notdef
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
#endif


#endif /* UTILS_H */
