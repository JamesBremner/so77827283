#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include "cRunWatch.h"

class cItem
{
public:
    cItem(const std::string &name)
        : myName(name)
    {
    }
    void add(
        const std::string &propname,
        const std::string &value);
    std::string propValue(int i) const
    {
        return myProps[i].second;
    }
    void addValue(
        const std::string &label,
        int value)
    {
        myValues.push_back(std::make_pair(label, value));
    }
    void display() const;

private:
    std::string myName;
    std::vector<std::pair<std::string, std::string>> myProps;
    std::vector<std::pair<std::string, int>> myValues;
};

class cLabel
{
public:
    void add(
        const std::string &n,
        int v,
        const std::string &test)
    {
        name = n;
        value.push_back(v);
        regex.push_back(test);
    }

    void test(cItem &item);

private:
    std::string name;
    std::vector<int> value;
    std::vector<std::string> regex;
};

void cItem::add(
    const std::string &propname,
    const std::string &value)
{
    myProps.push_back(std::make_pair(propname, value));
}

void cItem::display() const
{
    std::cout << "Item " << myName << "\n";
    for (auto &label : myValues)
    {
        std::cout << label.first;
        if (label.second == -1)
            std::cout << "X\t";
        else
            std::cout << std::to_string(label.second) << "\t";
    }
    std::cout << "\n";
}

void cLabel::test(cItem &item)
{
    for (int k = 0; k < value.size(); k++)
    {
        int value = -1;
        if (std::regex_match(
                item.propValue(0).c_str(),
                std::regex(regex[k].c_str())))
            value = k;

        item.addValue(name, value);
    }
}

std::vector<cItem> genItems1( int count )
{
    std::vector<cItem> ret;
    ret.push_back(cItem("I1"));
    ret.back().add("filename", "test");
    ret.push_back(cItem("I2"));
    ret.back().add("filename", "B");

    for (int k = 3; k < count; k++)
    {
        ret.push_back(cItem("I" + std::to_string(k)));
        ret.back().add("filename", "X");
    }
    return ret;
}
std::vector<cLabel> genConfig1( int count )
{
    std::vector<cLabel> ret;
    ret.push_back(cLabel());
    ret.back().add("A", 1, "(test)");
    ret.push_back(cLabel());
    ret.back().add("B", 1, "(B)");
    for (int k = 3; k < 100; k++)
    {
        ret.push_back(cLabel());
        ret.back().add("L" + std::to_string(k), 1, "(test)");
    }
    return ret;
}

void testLabels(
    std::vector<cLabel> &theConfig,
    std::vector<cItem> &theItems)
{
    raven::set::cRunWatch aWatcher("testLabels");

    for (auto &item : theItems)
        for (auto &test : theConfig)
        {
            test.test(item);
        }
}
void display(const std::vector<cItem> &theItems)
{
    for (auto &i : theItems)
    {
        i.display();
    }
}

main()
{
    // start time profiling
    raven::set::cRunWatch::Start();

    // generate config and items of required counts
    std::vector<cLabel> theConfig = genConfig1(100);
    std::vector<cItem> theItems = genItems1(1000);

    // do the tests
    testLabels( theConfig, theItems );

    // display the item labels
    display(theItems);

    // display time profiling report
    raven::set::cRunWatch::Report();

    return 0;
}
