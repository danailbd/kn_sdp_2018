#include <iostream>
#include <string>

#include <list>
#include <vector>

#include "include/Graph.h"


const int USER_MAX_NAME_LENGHT = 200;

class User
{
    public:
        // we are born with no friends
        User (const char* _name) {
            strcpy(name, _name);
        };
        virtual ~User () {};

        friend std::ostream& operator<<(std::ostream& os, const User& user) {
            os << user.name;
            return os;
        }

        friend bool operator==(const User& a, const User& b) {
            return !strcmp(a.name, b.name);
        }

        friend bool operator!=(User& a, const User& b) {
            return strcmp(a.name, b.name);
        }

    private:
        char name[USER_MAX_NAME_LENGHT]; // unique
};



//
// TODO implement
//
std::list<User> getFriendsRecommendations (Graph<User> friendRelations, User& lonelyUser, int maxRecommendations) {
    // TODO implement
    return std::list<User>();
}





/**********************************************************************
 *                               TESTS                                *
 **********************************************************************/

using std::cerr;
using std::cout;
using std::endl;

int tests_counter = 0;
int failed_tests_counter = 0;

template <class T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& l) {
    os << "[ ";
    for (auto i : l) {
        os << i << " ";
    }
    os << "]";
    return os;
}

class UnitTests {
    public:
        template <typename T>
        static void AssertEq(T expected, T value)
        {
            tests_counter++;
            if (expected != value) {
                cerr << "FAIL(" << tests_counter << "): Values mismatch!"
                    << "\n-- expected: " << expected << "\n---- actual: " << value << endl;
                failed_tests_counter++;
            } else {
                cout << "." << endl;
            }
        }

        static void PrintTestsResult() {
            if (failed_tests_counter)
                cout << "Failure: ";
            else
                cout << "Success: ";
            cout << tests_counter-failed_tests_counter << "|" << tests_counter << " tests." << endl;
        }
};


//
// XXX Not tested
//


void test_missingUser () {
    Graph<User> emptyGrapth;
    User invalidUser("some");

    std::list<User> result = getFriendsRecommendations(emptyGrapth, invalidUser, 10);
    std::list<User> expected; // empty

    UnitTests::AssertEq(expected, result);
}

void test_noFriends () {
    Graph<User> disjointGraph;
    User friendlessUser("some");
    User friendlessUser2("some");

    disjointGraph.addVertex(friendlessUser);
    disjointGraph.addVertex(friendlessUser2);

    std::list<User> result = getFriendsRecommendations(disjointGraph, friendlessUser, 10);
    std::list<User> expected; // empty

    UnitTests::AssertEq(expected, result);
}

void test_simpleSubFriends () {
    Graph<User> graph;
    // ivan - dragan - petkan - georgi
    User user("ivan");
    User amigo("dragan"); // friend is reserved word
    User sub_amigo("petkan");
    User sub_sub_amigo("georgi");
    User sub_sub_sub_amigo("nakoi tam");

    graph.addVertex(user);
    graph.addVertex(amigo);
    graph.addVertex(sub_amigo);
    graph.addVertex(sub_sub_amigo);
    graph.addVertex(sub_sub_sub_amigo);

    graph.addEdge(user, amigo);
    graph.addEdge(amigo, sub_amigo);
    graph.addEdge(sub_amigo, sub_sub_amigo);
    graph.addEdge(sub_sub_amigo, sub_sub_sub_amigo);


    std::list<User> result = getFriendsRecommendations(graph, user, 10);
    std::list<User> expected = { sub_amigo, sub_sub_amigo, sub_sub_sub_amigo }; // empty

    UnitTests::AssertEq(expected, result);

    // and with less recommendations...
    result = getFriendsRecommendations(graph, user, 2);
    expected = { sub_amigo, sub_sub_amigo }; // empty

    UnitTests::AssertEq(expected, result);
}

void test_commonSubFriendSimple () {
    Graph<User> graph;
    // ivan - dragan - petkan - georgi
    User user("ivan"),
         amigo1("dragan"), // friend is reserved word
         amigo2("dragan2"), // friend is reserved word
         sub_amigo1("petkan"),
         sub_amigo_common("petkan obshtiya"),
         sub_amigo3("petkan3");

    graph.addVertex(user);
    graph.addVertex(amigo1);
    graph.addVertex(amigo2);
    graph.addVertex(sub_amigo1);
    graph.addVertex(sub_amigo_common);
    graph.addVertex(sub_amigo3);

    graph.addEdge(user, amigo1);
    graph.addEdge(user, amigo2);
    graph.addEdge(amigo1, sub_amigo1);
    graph.addEdge(amigo1, sub_amigo_common);
    graph.addEdge(amigo2, sub_amigo_common);
    graph.addEdge(amigo2, sub_amigo3);


    std::list<User> result = getFriendsRecommendations(graph, user, 10);
    std::list<User> expected = { sub_amigo_common, sub_amigo1, sub_amigo3 }; // empty

    UnitTests::AssertEq(expected, result);
}

void test_commonFriendsComplex () {
    Graph<User> graph;
    User user("ivan"),
         amigo1("dragan1"),
         amigo2("dragan2"),
         sub_amigo1("petkan1"),
         sub_amigo_common("petkan obshtiya"),
         sub_amigo3("petkan3"),
         sub_sub_amigo1("georgi1"),
         sub_sub_amigo_2_common("joro"),
         sub_sub_amigo_3_common("prosto joro"),
         sub_sub_amigo4("georgi4");


    // save some space
    std::vector<User> tmp = {
        user, amigo1, amigo2, sub_amigo1,
        sub_amigo_common, sub_amigo3, sub_sub_amigo1,
        sub_sub_amigo_2_common, sub_sub_amigo_3_common, sub_sub_amigo4
    };

    for (auto user : tmp) {
        graph.addVertex(user);
    }
    // TODO ascii maybe?

    // build relations
    graph.addEdge(user, amigo1);
    graph.addEdge(user, amigo2);
    graph.addEdge(amigo1, sub_amigo1);
    graph.addEdge(amigo1, sub_amigo_common);
    graph.addEdge(amigo2, sub_amigo_common);
    graph.addEdge(amigo2, sub_amigo3);

    graph.addEdge(sub_amigo1, sub_sub_amigo1);
    graph.addEdge(sub_amigo1, sub_sub_amigo_2_common);
    graph.addEdge(sub_amigo1, sub_sub_amigo_3_common);

    graph.addEdge(sub_amigo_common, sub_sub_amigo_2_common);
    graph.addEdge(sub_amigo_common, sub_sub_amigo_3_common);

    graph.addEdge(sub_amigo3, sub_sub_amigo_3_common);
    graph.addEdge(sub_amigo3, sub_sub_amigo4);


    std::list<User> result = getFriendsRecommendations(graph, user, 10);
    std::list<User> expected = {
        sub_amigo_common, sub_amigo1, sub_amigo3, // level 1
        sub_sub_amigo_3_common, sub_sub_amigo_2_common, sub_sub_amigo1, sub_sub_amigo4 // level 2
    };

    UnitTests::AssertEq(expected, result);

    // again, test with restricted size
    result = getFriendsRecommendations(graph, user, 4);
    expected = {
        sub_amigo_common, sub_amigo1, sub_amigo3, // level 1
        sub_sub_amigo_3_common
    };

    UnitTests::AssertEq(expected, result);
}


void test_getRecommendedFriends() {
    test_missingUser();
    test_simpleSubFriends();
    test_commonSubFriendSimple();
    test_commonFriendsComplex();

    UnitTests::PrintTestsResult();
}

int main()
{
    test_getRecommendedFriends();

    return 0;
}
