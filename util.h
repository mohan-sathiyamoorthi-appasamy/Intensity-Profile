#ifndef UTIL_H
#define UTIL_H
//Print the file name and function name only if the project is compiled in debug mode. 
//#ifdef DEBUG
#define PRINTFUNCTIONNAME qDebug() << "Calling: " << __FILE__ << ":" << __FUNCTION__;
//#else 
//  #define PRINTFUNCTIONNAME { };
//#endif
#endif // UTIL_H