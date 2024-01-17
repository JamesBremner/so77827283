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
    std::vector<std::pair<std::string, std::string>> &
    getProps()
    {
        return myProps;
    }
    void addValue(
        const std::string &label,
        int value)
    {
        myValues.push_back(std::make_pair(label, value));
    }
    void display() const;

    std::string name() const{
        return myName;
    }

private:
    /// name of item
    std::string myName;

    /// properties: first = property name, second = property value
    std::vector<std::pair<std::string, std::string>> myProps;

    /// labels: first = label name, second = value
    std::vector<std::pair<std::string, int>> myValues;
};

class cLabel
{
public:
    void add(
        const std::string &n,
        int v,
        const std::string &test);

    void test(cItem &item);

    void display() const;

    int testCount() const
    {
        return value.size();
    }

private:
    std::string name;
    std::vector<int> value;
    std::vector<std::string> regex;
    std::vector<std::regex> regexCoded;
};

void cItem::add(
    const std::string &propname,
    const std::string &value)
{
    myProps.push_back(std::make_pair(propname, value));
}

void cItem::display() const
{
    std::cout << "Item " << myName;
    for (auto &p : myProps)
    {
        std::cout << " " << p.first << ", " << p.second;
    }
    std::cout << "\n";
    for (auto &label : myValues)
    {
        std::cout << label.first << ":";
        if (label.second == -1)
            std::cout << "X\t";
        else
            std::cout << std::to_string(label.second) << "\t";
    }
    std::cout << "\n";
}

void cLabel::add(
    const std::string &n,
    int v,
    const std::string &test)
{
    name = n;
    value.push_back(v);
    regex.push_back(test);
    regexCoded.push_back(std::regex(test));
}

void cLabel::test(cItem &item)
{
    //std::cout << "\ntesting item " << item.name() << "\n";
    for (int k = 0; k < value.size(); k++)
    {
        for (auto &prop : item.getProps())
        {
            // std::cout << "label " << name
            //     << " test " << regex[k];

            int value = -1;
            if (std::regex_match(
                    prop.second,
                    regexCoded[k]))
                value = k;

            // std::cout << " value " << k << "\n";
            item.addValue(name, value);
        }
    }
}
void cLabel::display() const
{
    std::cout << name << " ";
    for (int k = 0; k < value.size(); k++)
    {
        std::cout << value[k] << ": " << regex[k] << " ";
    }
    std::cout << "\n";
}

std::vector<cItem> genItems1(int count)
{
    std::vector<cItem> ret;
    ret.push_back(cItem("I1"));
    ret.back().add("filename", "test");
    ret.push_back(cItem("I2"));
    ret.back().add("filename", "B");
    ret.push_back(cItem("I3"));
    ret.back().add("filename", "test2");

    for (int k = 4; k < count; k++)
    {
        ret.push_back(cItem("I" + std::to_string(k)));
        ret.back().add("filename", "X");
    }
    return ret;
}
std::vector<cLabel> genConfig1(int count)
{
    std::vector<cLabel> ret;
    ret.push_back(cLabel());
    ret.back().add("A", 1, "(test)");
    ret.back().add("A", 2, "(test2)");
    ret.push_back(cLabel());
    ret.back().add("B", 1, "(B)");
    for (int k = 3; k < count; k++)
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
    std::cout << "\nThe items, with label values\n";

    for (auto &i : theItems)
    {
        i.display();
    }
}

void display(const std::vector<cLabel> &theLabels)
{
    std::cout << "The configuration ( tests )\n";
    for (auto &l : theLabels)
    {
        l.display();
    }
}

void timeProfileReport(
    const std::vector<cLabel> &theConfig,
    const std::vector<cItem> theItems)
{
    int count = 0;
    for (auto &t : theConfig)
        count += t.testCount();

    count *= theItems.size();

    std::cout << "\nTiming for " << count << " tests\n";

    raven::set::cRunWatch::Report();
}

main()
{
    // start time profiling
    raven::set::cRunWatch::Start();

    // generate config and items of required counts
    std::vector<cLabel> theConfig = genConfig1(50);
    display(theConfig);

    std::vector<cItem> theItems = genItems1(1000);

    // do the tests
    testLabels(theConfig, theItems);

    // display the item labels
    display(theItems);

    // display time profiling report
    timeProfileReport(theConfig, theItems);

    return 0;
}
