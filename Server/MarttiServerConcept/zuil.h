#ifndef ZUIL_H_
#define ZUIL_H_


class Zuil
{
    public:
        Zuil(int , string , string* );
        ~Zuil();
        void zetZoemer(bool);
        void zetLed(bool);
        void behaviour();

    private:
        bool inputButton;
        bool gasSensor;
        bool ledMode;
        bool zoemerMode;

        int port;
        Wemos wm;
        thread th;
        string prefix;
        string* commandLine;

        bool triggerCommands();
        void convertMessageToObjectAttr(char *);
        void commandCompare(string, void(Zuil::*)(bool), bool, bool*);


};

#endif // ZUIL_H_
