#include "log.h"

LogImgDataset::LogImgDataset()
{
    app = new log4cpp::FileAppender("fileAppender", "imgDatasetGen.log");
}

LogImgDataset::~LogImgDataset()
{

}

/*
 * type=0 ERROR
 * type=1 WARN
 * type=2 INFO
 */
void LogImgDataset::Log(int type, char *content)
{
    log4cpp::PatternLayout *plt = new log4cpp::PatternLayout();
    plt->setConversionPattern("[%d] %p %c %x: %m%n");
    //log4cpp::Appender *app = new log4cpp::FileAppender("fileAppender", "imgDatasetGen.log");
    app->setLayout(plt);

    log4cpp::Category &root = log4cpp::Category::getRoot().getInstance("Test");
    root.addAppender(app);
    root.setPriority(log4cpp::Priority::DEBUG);
    switch(type){
        case 0: root.error(content); break;
        case 1: root.warn(content); break;
        case 2: root.info(content); break;
        default: root.info(content); break;
    }
}


