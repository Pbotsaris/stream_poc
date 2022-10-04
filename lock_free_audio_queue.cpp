#include "lock_free_audio_queue.hpp"
#include <iostream>

LockFreeAudioQueue::LockFreeAudioQueue() : m_head(new Node) {
  m_tail = m_head.load();
};

LockFreeAudioQueue::~LockFreeAudioQueue() { // free queue
  while (const Node *old_head = m_head.load()) {
    m_head.store(old_head->m_next);
    delete old_head;
  }
};

void LockFreeAudioQueue::push(AudioPackage &&t_audio_package) {
  std::cout << "pushing...\n";
  auto new_data = std::make_shared<AudioPackage>(t_audio_package);
  Node *new_node = new Node;

  Node *const old_tail = m_tail.load();

  old_tail->m_data.swap(new_data); // new data tail
  old_tail->m_next = new_node;
  m_tail.store(new_node); // update new tail
};

std::shared_ptr<AudioPackage> LockFreeAudioQueue::pop() {

  Node *old_head = pop_head();

  if (!old_head) { // if empty return empty shared ptr
    return std::shared_ptr<AudioPackage>();
  }

  std::shared_ptr<AudioPackage> const result(old_head->m_data);
  delete old_head;
  return result;
}

LockFreeAudioQueue::Node *LockFreeAudioQueue::pop_head() {

  Node *const old_head = m_head.load();

  if (old_head == m_tail.load()) { // if head == tail
    return nullptr;
  }

  m_head.store(old_head->m_next); // pop from queue
  return old_head;               
}

bool LockFreeAudioQueue::empty() {

  Node *const old_head = m_head.load();
  if (old_head == m_tail.load()) {

    return true;
  } else {
    return false;
  }
}
