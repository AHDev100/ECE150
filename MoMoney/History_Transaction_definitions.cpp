#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  unsigned int month_date,  unsigned year_date, 
bool buy_sell_trans,  unsigned int number_shares,  double trans_amount){
    //Initialization and Declearation of necessary variables using parameter values
    symbol = ticker_symbol; 
    day = day_date; 
    month = month_date;  
    year = year_date;  
    shares = number_shares;   
    amount = trans_amount; 
    trans_id = assigned_trans_id;
    assigned_trans_id++; 
    if (buy_sell_trans){
      trans_type = "Buy";     
    }
    else if (!(buy_sell_trans)){
      trans_type = "Sell";
    }
    p_next = nullptr; 
}


// Destructor
// TASK 1
//
Transaction::~Transaction(){}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const &other){
  //Transaction operator overload checks the dates of two transaction objects to see which one (to left of operator) appeared earlier
  if (this->year < other.get_year()){
    return true; 
  }
  else if ((this->month < other.get_month())&&(this->year == other.get_year())){
    return true; 
  }
  else if ((this->month == other.get_month())&&(this->year == other.get_year())&&(this->day < other.get_day())){
    return true; 
  }
  else if ((this->month == other.get_month())&&(this->year == other.get_year())&&(this->day == other.get_day())){
    if (this->get_trans_id() > other.get_trans_id()){
      return true; 
    }
  }
  return false; 
  //If the lefthand transaction occurred before right hand, we return true, otherwise, we return false
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History(){
  p_head = nullptr; //For the linkedlist to be empty, the head (the first pointer) will be a nullptr
}

// Destructor
// TASK 3
//
History::~History(){
  Transaction *tmp{}; //Temporary pointer for storing address of each node
  while (p_head != nullptr){ //When the address of p_head is NULL, we have reached end of the linkedlist
    tmp = p_head; //We give the address stored in p_head to tmp
    p_head = p_head->get_next(); //We set the address of p_head to the address pointing to the next node in the linkedlist
    delete tmp; //We delete the value stored in tmp - we don't want to mess with our p_head (repeated use of delete operator deallocates the dynamically allocated linkedlist)
  }
}


// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
  ece150::open_file(); //We call open_file function in ece150 namespace to open the file for reading it
  Transaction *new_trans{}; //We create a new Transaction object and new_trans is a pointer to that object 
  while(ece150::next_trans_entry()){ //While we are on the line (which contains the information) - when we move to next line, the condition is false
    new_trans = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()); //We create a new transaction object using the helper functions defined in 150 - for giving us the data value members for the new transaction 
    History::insert(new_trans); //We insert our new transaction object as a new node at the end of the linked list
  }
  ece150::close_file(); //Now that we are done working with the file, we close it using the close_file function
}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){ 
  //use the get_next() and set_next() functions 
  if (p_head == nullptr){ //If the head is a nullptr, we now commence the creation of the linkedlist by assigning p_new_trans to be the head
    p_head = p_new_trans; 
  }
  else {
    Transaction *insert_trans{p_head}; //We initialize another pointer to a new transaction object to have the same address as our head
    while(insert_trans->get_next() != NULL){ //If the next node is a nullptr, we have reached the end of the linkedlist, which is where we want to store the *p_new_trans Transaction object
      insert_trans = insert_trans->get_next(); //We set insert_trans to be the next node in the list - we are traversing through the list, from start to finish
    }
    insert_trans->set_next(p_new_trans); //Sets the address of insert_trans (which is at the end of the list) to that of the address of p_new_trans 
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){

}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
  Transaction *list_node{p_head}; //Current location in the linked list
  double acb_total{0}; //acb accumulator 
  double acb_per_share{0}; //variable to calculate acb_per_share
  unsigned int share_balance_total{0}; //gives us the total number of shares
  double cgl{0}; //To compute the cgl for each cgl transaction
  while (list_node != NULL){
    if (list_node->get_trans_type()){ //If this condition is true, the transaction is a "Buy" transaction
      acb_total += (list_node -> get_amount()); //Add the amount paid to give the total acb
      list_node -> set_acb(acb_total); //We set the acb of that transaction to acb_total 
      share_balance_total += (list_node -> get_shares()); //The number of shares in the transaction is added to the total share balance
      list_node -> set_share_balance(share_balance_total); //We set the share_balance of the transaction to the updated total share_balance
      acb_per_share += (list_node -> get_acb())/(list_node -> get_share_balance()); //The quotient of the new acb and the new share balance are added to the acb_per_share variable
      list_node -> set_acb_per_share(acb_per_share); //We set the acb_per_share of the transaction to the new value of the acb per share
      list_node = list_node -> get_next(); //We move to the next node in the transaction list
    }
    else if (!(list_node->get_trans_type())){ //If this is true, the transaction is of type "Sell"
      acb_total -= ((list_node -> get_shares())*(acb_per_share)); //We subtract the product of the current number of shares and the acb_per_share of the previous transaction to get the new acb
      list_node -> set_acb(acb_total); //We update the value of the acb
      cgl = ((list_node->get_amount()) - ((list_node -> get_shares())*acb_per_share)); //We implement the formula for the cgl of the current transaction
      list_node -> set_cgl(cgl); //we set the new cgl value for the current transaction
      share_balance_total -= (list_node -> get_shares()); //We subtract by the number of shares for sell transaction
      list_node -> set_share_balance(share_balance_total); //Set this value as the new share balance
      acb_per_share += (list_node -> get_acb())/(list_node -> get_share_balance()); //Acb per share is the acb divided by the total share balance
      list_node -> set_acb_per_share(acb_per_share); //Set the acb to this new value
      list_node = list_node -> get_next(); //Move to the next node
    }
  }
} 


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year){
  Transaction *list_loc{p_head}; //location in the linked list
  double cgl_total{0}; //total cgl 
  double acb_per_share{0}; //total acb per share
  double acb_tot{0}; //total acb value
  unsigned int share_bal{0}; //total share balance
  double cgl_lone{0}; //each individual cgl transaction value 
  while (list_loc != NULL){
    if (list_loc -> get_trans_type()){ //If the transaction is of type "Buy"
      acb_tot += (list_loc -> get_amount()); //Update the total acb by adding the amount of the buy
      list_loc -> set_acb(acb_tot); //Set this to be the acb value for the transaction
      share_bal += (list_loc -> get_shares()); //The number of shares in the transaction is added to the total share balance
      list_loc -> set_share_balance(share_bal); //We set the share_balance of the transaction to the updated total share_balance
      acb_per_share += (list_loc -> get_acb())/(list_loc -> get_share_balance()); //The quotient of the new acb and the new share balance are added to the acb_per_share variable
      list_loc -> set_acb_per_share(acb_per_share); //We set the acb_per_share of the transaction to the new value of the acb per share
      list_loc = list_loc->get_next(); //We move to the next node
    } 
    else if (!(list_loc -> get_trans_type())){ //Sell transaction case
      acb_tot -= ((list_loc -> get_shares())*(acb_per_share)); //We subtract the product of the current number of shares and the acb_per_share of the previous transaction to get the new acb
      list_loc -> set_acb(acb_tot); //We update the acb value  
      cgl_lone = ((list_loc->get_amount()) - ((list_loc -> get_shares())*acb_per_share)); //We implement the formula for the cgl of the current transaction
      cgl_total += cgl_lone; //We add this cgl value to our cgl total accumulator 
      list_loc -> set_cgl(cgl_lone); //We update the cgl value to be that of the lone value as needed
      share_bal -= (list_loc -> get_shares()); //We subtract the number of shares sold from our total shares
      list_loc -> set_share_balance(share_bal); //We update the share balance
      acb_per_share += (list_loc -> get_acb())/(list_loc -> get_share_balance()); //We divide the new acb by the new share_balance and add it to our accumulator 
      list_loc -> set_acb_per_share(acb_per_share); //set the new acb per share to be value of the accumulator
      list_loc = list_loc -> get_next(); //We move to the next node
    }
  }
  return cgl_total; //return the accumulated cgl for all the transactions combined
}


// print() Print the transaction history.
//TASK 9
//
void History::print(){ 
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl; 
  Transaction *details = p_head; //The details pointer is given the same address as the head of the linked_list 
  while(details != NULL){ //When it is NULL, we have reached the end of the linked lsit
    details->print(); //We call the print() helper function to print out the details for that node in the list
    details = details->get_next(); //We move onto the next node, so that we can print out the data stored there
  }
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl; 
} 

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
