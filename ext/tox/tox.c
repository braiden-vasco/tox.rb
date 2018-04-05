#include "tox.h"

#if !(TOX_VERSION_IS_API_COMPATIBLE(0, 2, 1))
  #error "Tox API version is not compatible"
#endif

// Instances

VALUE mTox;

VALUE mTox_eNullError;
VALUE mTox_eUnknownError;

VALUE mTox_mVersion;
VALUE mTox_mUserStatus;
VALUE mTox_cOptions;
VALUE mTox_cClient;
VALUE mTox_cNode;
VALUE mTox_cFriend;
VALUE mTox_cAddress;
VALUE mTox_cNospam;
VALUE mTox_cPublicKey;
VALUE mTox_mOutMessage;

VALUE mTox_mUserStatus_NONE;
VALUE mTox_mUserStatus_AWAY;
VALUE mTox_mUserStatus_BUSY;

VALUE mTox_cClient_eBadSavedataError;

VALUE mTox_cFriend_eNotFoundError;
VALUE mTox_cFriend_eNotConnectedError;
VALUE mTox_cFriend_cOutMessage;

VALUE mTox_mOutMessage_eSendQueueAllocError;
VALUE mTox_mOutMessage_eTooLongError;
VALUE mTox_mOutMessage_eEmptyError;

// Singleton methods

static VALUE mTox_hash(VALUE self, VALUE data);

/*************************************************************
 * Initialization
 *************************************************************/

void Init_tox()
{
  if (!TOX_VERSION_IS_ABI_COMPATIBLE()) {
    rb_raise(rb_eLoadError, "incompatible Tox ABI version");
  }

  // Instances

  mTox = rb_const_get(rb_cObject, rb_intern("Tox"));

  mTox_eNullError    = rb_const_get(mTox, rb_intern("NullError"));
  mTox_eUnknownError = rb_const_get(mTox, rb_intern("UnknownError"));

  mTox_mVersion    = rb_const_get(mTox, rb_intern("Version"));
  mTox_mUserStatus = rb_const_get(mTox, rb_intern("UserStatus"));
  mTox_cOptions    = rb_const_get(mTox, rb_intern("Options"));
  mTox_cClient     = rb_const_get(mTox, rb_intern("Client"));
  mTox_cNode       = rb_const_get(mTox, rb_intern("Node"));
  mTox_cFriend     = rb_const_get(mTox, rb_intern("Friend"));
  mTox_cAddress    = rb_const_get(mTox, rb_intern("Address"));
  mTox_cNospam     = rb_const_get(mTox, rb_intern("Nospam"));
  mTox_cPublicKey  = rb_const_get(mTox, rb_intern("PublicKey"));
  mTox_mOutMessage = rb_const_get(mTox, rb_intern("OutMessage"));

  mTox_mUserStatus_NONE = rb_const_get(mTox_mUserStatus, rb_intern("NONE"));
  mTox_mUserStatus_AWAY = rb_const_get(mTox_mUserStatus, rb_intern("AWAY"));
  mTox_mUserStatus_BUSY = rb_const_get(mTox_mUserStatus, rb_intern("BUSY"));

  mTox_cClient_eBadSavedataError = rb_const_get(mTox_cClient, rb_intern("BadSavedataError"));

  mTox_cFriend_eNotFoundError     = rb_const_get(mTox_cFriend, rb_intern("NotFoundError"));
  mTox_cFriend_eNotConnectedError = rb_const_get(mTox_cFriend, rb_intern("NotConnectedError"));
  mTox_cFriend_cOutMessage        = rb_const_get(mTox_cFriend, rb_intern("OutMessage"));

  mTox_mOutMessage_eSendQueueAllocError = rb_const_get(mTox_mOutMessage, rb_intern("SendQueueAllocError"));
  mTox_mOutMessage_eTooLongError        = rb_const_get(mTox_mOutMessage, rb_intern("TooLongError"));
  mTox_mOutMessage_eEmptyError          = rb_const_get(mTox_mOutMessage, rb_intern("EmptyError"));

  // Singleton methods

  rb_define_singleton_method(mTox, "hash", mTox_hash, 1);

  mTox_mVersion_INIT();
  mTox_cOptions_INIT();
  mTox_cClient_INIT();
  mTox_cFriend_INIT();
}

/*************************************************************
 * Singleton methods
 *************************************************************/

// Tox.hash
VALUE mTox_hash(const VALUE self, const VALUE data)
{
  Check_Type(data, T_STRING);

  const uint8_t result[TOX_HASH_LENGTH];

  if (true != tox_hash(result, RSTRING_PTR(data), RSTRING_LEN(data))) {
    RAISE_FUNC_RESULT("tox_hash");
  }

  const VALUE hash = rb_str_new(result, TOX_HASH_LENGTH);

  return hash;
}
