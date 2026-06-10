# include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>
#include <algorithm>
#include "thread_pool.hpp"
namespace {
bool no_color = false;
bool use_stdout = true;
bool use_log = true;

enum class ansi:std::uint8_t
{
    reset = 0,
    bold = 1,
    dim = 2,
    italic = 3,
    underline = 4,
    invert = 7,
    strike = 9,
    double_underline = 21,
    normal_intensity = 22,
    not_italic = 23,
    not_underlined = 24,
    fg_black = 30,
    fg_red = 31,
    fg_green = 32,
    fg_yellow = 33,
    fg_blue = 34,
    fg_magenta = 35,
    fg_cyan = 36,
    fg_white = 37,
    bg_black = 40,
    bg_red = 41,
    bg_green = 42,
    bg_yellow = 43,
    bg_blue = 44,
    bg_magenta = 45,
    bg_cyan = 46,
    bg_white = 47,
    fg_bright_black = 90,
    fg_bright_red = 91,
    fg_bright_green = 92,
    fg_bright_yellow = 93,
    fg_bright_blue = 94,
    fg_bright_magenta = 95,
    fg_bright_cyan = 96,
    fg_bright_white = 97,
    bg_bright_black = 100,
    bg_bright_red = 101,
    bg_bright_green = 102,
    bg_bright_yellow = 103,
    bg_bright_blue = 104,
    bg_bright_magenta = 105,
    bg_bright_cyan = 106,
    bg_bright_white = 107
};

constexpr std::initializer_list<ansi> ansi_error = {ansi::fg_bright_red};
constexpr std::initializer_list<ansi> ansi_separator = {ansi::fg_bright_yellow};
template <typename... T>
void logln(T &&...items)
{
    log(std::forward<T>(items)..., '\n');
}
template <typename... T>
void logln_ansi(const std::initializer_list<ansi> codes, T&&... items)
{
    log_ansi(codes, std::forward<T>(items)..., '\n');
}

template<typename... T>
void log(const T&... items){
    if(use_stdout){
        sync_cout.print(items...);//线程安全的屏幕打印
    }
    if(use_log){
        sync_log.print(items...);//线程安全的文件写入
    }
}

void print_header(const std::string_view& text,const char symbol='=',const std::initializer_list<ansi>codes = ansi_separator){
    const std::string separator(text.length(),symbol);
    logln_ansi(codes,BS::synced_stream::flush,"\n",separator,'\n',text,'\n',separator);
}
class [[nodiscard]] arg_parser{
public:
    arg_parser(int argc,char* argv[]):args(argv+1,argv+argc),executable(argv[0]){};

    [[nodiscard]] bool operator[](const std::string_view arg){
        if(size() > 0){
            return (args.count(arg) == 1);
        }
        return allowed[arg].def;
    }
    [[nodiscard]] std::size_t size() const{
        return args.size();
    }
    void add_argument(const std::string_view arg,const std::string_view desc,const bool def){
        allowed[arg] = {desc, def};
    }

    std::string_view get_executable() {
        return executable;
    }

    void show_help(){
        int width = 1;
        for(const auto& [arg,opt]:allowed){
            width = std::max(width, static_cast<int>(arg.size()));
        }
        logln("\nAvailable options:(all are on/off and defaults to off)");
        for(const auto& [arg,opt]:allowed){
            logln(" ",std::left,std::setw(width),arg," ",opt.desc);
        }
        log("If not options are entered,the default is:\n");
        for(const auto&[arg,opt]:allowed){
            if(opt.def)
                log(arg, " ");
        }
        logln();
    }
    [[nodiscard]] bool verify()const{
        return std::all_of(args.begin(),args.end(),[this](const std::string_view& arg)
        {
            return allowed.count(arg) == 1;
        });
    }

private:
    std::set<std::string_view> args;
    std::string_view executable;
    struct arg_spec{
        std::string_view desc;
        bool def = false;
    };
    std::map<std::string_view, arg_spec> allowed;
};

void check_constructor(){
    BS::thread_pool pool;
}
}
int main(int argc, char* argv[])
{
    no_color = (std::getenv("NO_COLOR") != nullptr);
    std::map<std::string, bool> defaults;

    std::ifstream default_args_file("default_args.txt");
    if(!default_args_file.is_open())
        default_args_file.open("../default_args.txt");
    if(default_args_file.is_open()){
        std::string line;
        std::getline(default_args_file, line);
        std::istringstream iss(line);
        std::string arg;
        while(iss >> arg)
            defaults[arg] = true;
        default_args_file.close();
    }else{ //默认参数
        defaults = {{"help", false}, {"stdout", true}, {"log", true}, {"tests", true}, {"deadlock", false}, {"benchmarks", true}, {"plot", false}, {"save", false}};
    }

    // 解释命令参数
    arg_parser args(argc, argv);
    args.add_argument("help", "show this help message and exit", defaults["help"]);
    args.add_argument("stdout","print to the standard output",defaults["stdout"]);
    args.add_argument("log", "print to the log file", defaults["log"]);
    args.add_argument("tests","perform standard tests",defaults["tests"]);
    args.add_argument("deadlock","perform long deadlock tests",defaults["deadlock"]);
    args.add_argument("benchmark", "perform full mandelbort plot benchmark", defaults["benchmark"]);
    args.add_argument("plot", "perform quick Mandelbrot plot benchmarks", defaults["plot"]);
    args.add_argument("save", "save the mandelbrot plot to a file", defaults["save"]);

    if(args.size() > 0){
        if(args["help"]||!args.verify()){
            // show_intro();
            args.show_help();
            return 0;
        }
        if(!args["stdout"]  && !args["log"]){
            args.show_help();
            logln_ansi(ansi_error,"ERROR:No output stream specified ! please enter one or more of :log,stdout abouting");
        }
        if(!args["benchmarks"] && !args["deadlock"] && !args["plot"] && !args["tests"]){
            args.show_help();
            logln_ansi(ansi_error,"\nError:No test or benchmark required ! please enter one or more of :benchmarks,deadlock,plot,tests,Abouting");
            return 0;
        }
    }
    std::ofstream log_file;
    // sync_log.remove_streams(std::cout);


    if(args["tests"]){
        print_header("checking the constructors");
        check_constructor();
    }
    return 0;
}