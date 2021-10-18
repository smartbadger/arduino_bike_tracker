#ifndef DEBUG_H
#define DEBUG_H

class Debug
{

	public:
		Debug(boolean active);
        ~Debug();
        void print(char value);
        void setup();
    private:
        boolean active;
};

#endif