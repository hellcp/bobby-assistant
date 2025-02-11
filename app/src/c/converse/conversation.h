/*
 * Copyright 2025 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <pebble.h>

typedef struct Conversation Conversation;
typedef struct ConversationEntry ConversationEntry;

typedef enum {
  ConversationActionTypeSetAlarm,
  ConversationActionTypeSetReminder,
  ConversationActionTypeUpdateChecklist,
  ConversationActionTypePokeHomeAssistant,
  ConversationActionTypeGenericSentence,
} ConversationActionType;

typedef struct {
  time_t time;
  bool is_timer;
  bool deleted;
} ConversationActionSetAlarm;

typedef struct {
  time_t time;
} ConversationActionSetReminder;

typedef struct {
} ConversationActionPokeHomeAssistant;

typedef struct {
  char *sentence;
} ConversationActionGenericSentence;

typedef struct {
  ConversationActionType type;
  union {
    ConversationActionSetAlarm set_alarm;
    ConversationActionSetReminder set_reminder;
    ConversationActionPokeHomeAssistant poke_hass;
    ConversationActionGenericSentence generic_sentence;
  } action;
} ConversationAction;

typedef struct {
  char *prompt;
} ConversationPrompt;

typedef struct {
  char *response;
  size_t len;
  size_t allocated;
  bool complete;
} ConversationResponse;

typedef struct {
  char *thought;
} ConversationThought;

typedef struct {
  char *message;
} ConversationError;

typedef enum {
  EntryTypePrompt,
  EntryTypeResponse,
  EntryTypeThought,
  EntryTypeAction,
  EntryTypeError,
} EntryType;

Conversation* conversation_create();
void conversation_destroy(Conversation* conversation);
void conversation_add_prompt(Conversation* conversation, const char* prompt);
void conversation_add_response(Conversation* conversation, const char* response);
void conversation_start_response(Conversation *conversation);
bool conversation_add_response_fragment(Conversation *conversation, const char* fragment);
void conversation_complete_response(Conversation *conversation);
void conversation_add_thought(Conversation* conversation, char* thought);
void conversation_add_action(Conversation* conversation, ConversationAction* action);
void conversation_add_error(Conversation* conversation, const char* error_text);
void conversation_set_thread_id(Conversation* conversation, const char* thread_id);
const char* conversation_get_thread_id(Conversation* conversation);
int conversation_length(Conversation* conversation);
bool conversation_is_idle(Conversation* conversation);
ConversationEntry* conversation_entry_at_index(Conversation* conversation, int index);
ConversationEntry* conversation_peek(Conversation* conversation);
EntryType conversation_entry_get_type(ConversationEntry* entry);

ConversationPrompt* conversation_entry_get_prompt(ConversationEntry* entry);
ConversationResponse* conversation_entry_get_response(ConversationEntry* response);
ConversationThought* conversation_entry_get_thought(ConversationEntry* thought);
ConversationError* conversation_entry_get_error(ConversationEntry* response);
ConversationAction* conversation_entry_get_action(ConversationEntry* action);

#endif
