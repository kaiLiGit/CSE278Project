/* 
 * File:   pokemonCatalog.cpp
 * Author: Kai Li 
 *
 * Copyright 2016 mygitacc50@gmail.com/
 */

#include <boost/asio.hpp>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>

// A shortcut for a map of string, string to store 
// identifier and messages associated with it
using StrStrMap = std::unordered_map<std::string, std::string>;

// The name of the file in which messages are to be stored
const std::string DataFile = "./pokedex.txt";

// The name of the file that contains valid pokemon names.
const std::string PokemonDBFile = "pokemons.txt";

// The standard 200 OK message to display
const std::string OKmsg = "200 OK\n";

// The standard 404 Not found error message to display
const std::string NotFoundMsg = " 404 Not Found\n";

// A shared unordered map to store identifiers and messages
StrStrMap pokedex;

// The database of valid pokemon names
StrStrMap pokeDB;

/**
 * Convenience method to load valid pokemon names into pokeDB hash map.
 */
void loadPokeDB() {
    std::ifstream db(PokemonDBFile);
    if (!db.good()) {
        std::cerr << "Error loading data from " << PokemonDBFile << std::endl;
    } else {
        std::string pokemon;
        while (db >> pokemon) {
            pokeDB[pokemon] = "1";  // add an entry
        }
    }
}

/**
 * Convenience method to check-and-add pokemon name and information 
 * to pokedex.
 *
 * @param id The name of the pokemon. If the name is invalid
 * this method prints "406 Not Acceptable".
 * @param info The information associated with the pokemon. 
 * @param os The output stream to write the data.
 * @param logMsg Flag to indicate if log messages are to be added.
 */
void put(const std::string& id, const std::string& info, 
        std::ostream& os, bool logMsg = true) {
    if (pokeDB.find(id) != pokeDB.end()) {
        pokedex[id] = info;  // store information in pokedex
        if (logMsg) {
            os << "201 Created\n";
        }
    } else {
        os << "406 Not Acceptable\n";
    }
}

/**
 * Method to find an entry in the pokedex
 * 
 * @param pokeName The name of the pokemon
 * @param os The output stream to report information/error
 */
void get(const std::string& pokeName, std::ostream& os) {
    if (pokedex.find(pokeName) != pokedex.end()) {
        os << pokeName << " " << pokedex[pokeName] << std::endl
           << OKmsg;
    } else {
        // os << pokeName << " 404 Not Found\n";
        os << "404 Not Found\n";
    }
}

/**
 * Removing a pokemon from the pokedex.
 * @param pokeName The name of the pokemon.
 * @param os The output stream to report information/error
 */
void erase(const std::string& pokeName, std::ostream& os) {
    if (pokedex.find(pokeName) != pokedex.end()) {
        pokedex.erase(pokeName);
        os << OKmsg;
    } else {
        os << pokeName << " 404 Not Found\n";
    }
}

/** 
 * Convenience method to save the pokedex to a given file
 */
void save(std::ostream& os) {
    std::ofstream outFile(DataFile);
    for (auto entry : pokedex) {
        outFile << entry.first << " " << entry.second << std::endl;
    }
    os << OKmsg;
}

/** 
 * Convenience method to load the pokedex from a given file
 */
void load(std::ostream& os) {
    pokedex.clear();  // clear the pokedex
    std::ifstream inFile(DataFile);
    std::string pokeName, info;
    // Repeatedly load entries until EOF...
    while (inFile >> pokeName) {
        std::getline(inFile, info);
        put(pokeName, info.substr(1), os, false);
    }
    os << OKmsg;
}

void find(const std::string& name, std::ostream& os) {
    for (auto entry : pokedex) {
        if (entry.first.find(name) != std::string::npos) {
            os << entry.first << " " << entry.second << std::endl;
        }
    }
    os << OKmsg;
}

/**
 * Convenience method to read a line from a given stream.
 * @param is The input stream from where a line is to be read.
 * @return A line of input.
 */
std::string readLine(std::istream& is) {
    // Read a line of the command
    std::string line;
    std::getline(is, line);
    // Remove trailing '\r' for compatibility with windows
    if (line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

/** Convenience method to process line-by-line of a commands.
 * 
 * This method is a convenience method to process line-by-line of commands.
 * 
 * \param[in,out] is The input stream from where to process commands.
 * 
 * \param[in,out] out The output stream to where data is to be written.
 * 
 */
void processCmds(std::istream& is, std::ostream& os) {
    std::string cmd;
    // Read line-by-line and process commands
    do {
        std::string line = readLine(is);
        // Get index of first & 2nd word in line to get command & id
        const size_t spc1 = line.find(' ');
        const size_t spc2 = line.find(' ', spc1 + 1);
        cmd = line.substr(0, spc1);
        // There maybe be an id/key as second word.
        std::string id = line.substr(spc1 + 1, spc2 - spc1 - 1);
        // Use helper method to perform operations for various commands
        if (cmd == "GET") {
            get(id, os);
        } else if (cmd == "PUT") {
            put(id, line.substr(spc2 + 1), os);
        } else if (cmd == "DELETE") {
            erase(id, os);
        } else if (cmd == "SAVE") {
            save(os);  // save pokedex
        } else if (cmd == "LOAD") {
            load(os);  // load pokedex
        } else if (cmd == "FIND") {
            find(id, os);
        } else if (cmd != "QUIT") {
            os << "400 Bad Request\n";
        }
    } while (cmd != "QUIT");
}

/*
 * The main method that coordinates various operations of this program.
 */
int main(int argc, char *argv[]) {
    // Load pokemon names into pokeDB
    loadPokeDB();
    if (argc < 2) {
        processCmds(std::cin, std::cout);
    } else {
        using namespace boost::asio;
        using namespace boost::asio::ip;
        io_service io_service;  // generic service
        tcp::endpoint endpoint(tcp::v4(), 0);  // 0 == pick a free port
        tcp::acceptor server(io_service, endpoint);  // create a socket
        std::cout << "Listening on port " << server.local_endpoint().port() 
                  << std::endl;
        ip::tcp::iostream client;  // client stream
        server.accept(*client.rdbuf());  // wait for client to connect

        processCmds(client, client);
    }
    return 0;
}
