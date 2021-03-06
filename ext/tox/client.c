#include "tox.h"

#include <time.h>

// Memory management
static VALUE mTox_cClient_alloc(VALUE klass);
static void  mTox_cClient_free(mTox_cClient_CDATA *free_cdata);

// Public methods

static VALUE mTox_cClient_iteration_interval(VALUE self);
static VALUE mTox_cClient_iterate(VALUE self);

static VALUE mTox_cClient_public_key(VALUE self);
static VALUE mTox_cClient_address(VALUE self);
static VALUE mTox_cClient_nospam(VALUE self);
static VALUE mTox_cClient_nospam_ASSIGN(VALUE self, VALUE nospam);
static VALUE mTox_cClient_savedata(VALUE self);
static VALUE mTox_cClient_udp_port(VALUE self);
static VALUE mTox_cClient_tcp_port(VALUE self);

static VALUE mTox_cClient_bootstrap(VALUE self, VALUE address, VALUE port, VALUE public_key);
static VALUE mTox_cClient_add_tcp_relay(VALUE self, VALUE address, VALUE port, VALUE public_key);

static VALUE mTox_cClient_name(VALUE self);
static VALUE mTox_cClient_name_ASSIGN(VALUE self, VALUE name);

static VALUE mTox_cClient_status(VALUE self);
static VALUE mTox_cClient_status_ASSIGN(VALUE self, VALUE status);

static VALUE mTox_cClient_status_message(VALUE self);
static VALUE mTox_cClient_status_message_ASSIGN(VALUE self, VALUE status_message);

static VALUE mTox_cClient_friend_numbers(VALUE self);

static VALUE mTox_cClient_friend_add_norequest(VALUE self, VALUE public_key);
static VALUE mTox_cClient_friend_add(VALUE self, VALUE address, VALUE text);

// Private methods

static VALUE mTox_cClient_initialize_with(VALUE self, VALUE options);

/*************************************************************
 * Initialization
 *************************************************************/

void mTox_cClient_INIT()
{
  // Memory management
  rb_define_alloc_func(mTox_cClient, mTox_cClient_alloc);

  // Public methods

  rb_define_method(mTox_cClient, "iteration_interval", mTox_cClient_iteration_interval, 0);
  rb_define_method(mTox_cClient, "iterate",            mTox_cClient_iterate,            0);

  rb_define_method(mTox_cClient, "public_key", mTox_cClient_public_key,    0);
  rb_define_method(mTox_cClient, "address",    mTox_cClient_address,       0);
  rb_define_method(mTox_cClient, "nospam",     mTox_cClient_nospam,        0);
  rb_define_method(mTox_cClient, "nospam=",    mTox_cClient_nospam_ASSIGN, 1);
  rb_define_method(mTox_cClient, "savedata",   mTox_cClient_savedata,      0);
  rb_define_method(mTox_cClient, "udp_port",   mTox_cClient_udp_port,      0);
  rb_define_method(mTox_cClient, "tcp_port",   mTox_cClient_tcp_port,      0);

  rb_define_method(mTox_cClient, "bootstrap",     mTox_cClient_bootstrap,     3);
  rb_define_method(mTox_cClient, "add_tcp_relay", mTox_cClient_add_tcp_relay, 3);

  rb_define_method(mTox_cClient, "name",  mTox_cClient_name,        0);
  rb_define_method(mTox_cClient, "name=", mTox_cClient_name_ASSIGN, 1);

  rb_define_method(mTox_cClient, "status",  mTox_cClient_status,        0);
  rb_define_method(mTox_cClient, "status=", mTox_cClient_status_ASSIGN, 1);

  rb_define_method(mTox_cClient, "status_message",  mTox_cClient_status_message,        0);
  rb_define_method(mTox_cClient, "status_message=", mTox_cClient_status_message_ASSIGN, 1);

  rb_define_method(mTox_cClient, "friend_numbers", mTox_cClient_friend_numbers, 0);

  rb_define_method(mTox_cClient, "friend_add_norequest", mTox_cClient_friend_add_norequest, 1);
  rb_define_method(mTox_cClient, "friend_add",           mTox_cClient_friend_add,           2);

  // Private methods

  rb_define_private_method(mTox_cClient, "initialize_with", mTox_cClient_initialize_with, 1);
}

/*************************************************************
 * Memory management
 *************************************************************/

VALUE mTox_cClient_alloc(const VALUE klass)
{
  mTox_cClient_CDATA *alloc_cdata = ALLOC(mTox_cClient_CDATA);

  alloc_cdata->tox = NULL;

  return Data_Wrap_Struct(klass, NULL, mTox_cClient_free, alloc_cdata);
}

void mTox_cClient_free(mTox_cClient_CDATA *const free_cdata)
{
  if (free_cdata->tox) {
    tox_kill(free_cdata->tox);
  }

  free(free_cdata);
}

/*************************************************************
 * Public methods
 *************************************************************/

// Tox::Client#iteration_interval
VALUE mTox_cClient_iteration_interval(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  uint32_t iteration_interval_msec_data =
    tox_iteration_interval(self_cdata->tox);

  const double iteration_interval_sec_data =
    ((double)iteration_interval_msec_data) * 0.001;

  const VALUE iteration_interval_sec = DBL2NUM(iteration_interval_sec_data);

  return iteration_interval_sec;
}

// Tox::Client#iterate
VALUE mTox_cClient_iterate(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  tox_iterate(self_cdata->tox, self);

  return Qnil;
}

// Tox::Client#public_key
VALUE mTox_cClient_public_key(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  char public_key_data[TOX_PUBLIC_KEY_SIZE];

  tox_self_get_public_key(self_cdata->tox, public_key_data);

  const VALUE public_key_value =
    rb_str_new(public_key_data, TOX_PUBLIC_KEY_SIZE);

  const VALUE public_key = rb_funcall(mTox_cPublicKey, rb_intern("new"), 1,
                                      public_key_value);

  return public_key;
}

// Tox::Client#address
VALUE mTox_cClient_address(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  char address_data[TOX_ADDRESS_SIZE];

  tox_self_get_address(self_cdata->tox, address_data);

  const VALUE address_value = rb_str_new(address_data, TOX_ADDRESS_SIZE);

  const VALUE address = rb_funcall(mTox_cAddress, rb_intern("new"), 1,
                                   address_value);

  return address;
}

// Tox::Client#nospam
VALUE mTox_cClient_nospam(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  uint32_t nospam_data = tox_self_get_nospam(self_cdata->tox);

  const VALUE nospam_as_integer = LONG2FIX(nospam_data);

  const VALUE nospam = rb_funcall(mTox_cNospam, rb_intern("new"), 1,
                                  nospam_as_integer);

  return nospam;
}

// Tox::Client#nospam
VALUE mTox_cClient_nospam_ASSIGN(const VALUE self, const VALUE nospam)
{
  if (!rb_funcall(nospam, rb_intern("is_a?"), 1, mTox_cNospam)) {
    RAISE_TYPECHECK("Tox::Client#nospam=", "nospam", "Tox::Nospam");
  }

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const VALUE nospam_as_integer = rb_funcall(nospam, rb_intern("to_i"), 0);

  tox_self_set_nospam(
    self_cdata->tox,
    FIX2LONG(nospam_as_integer)
  );

  return Qnil;
}

// Tox::Client#savedata
VALUE mTox_cClient_savedata(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  size_t data_size = tox_get_savedata_size(self_cdata->tox);
  char *data = ALLOC_N(char, data_size);

  tox_get_savedata(self_cdata->tox, data);

  const VALUE savedata = rb_str_new(data, data_size);

  return savedata;
}

// Tox::Client#udp_port
VALUE mTox_cClient_udp_port(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  TOX_ERR_GET_PORT error;

  const uint16_t udp_port_data = tox_self_get_udp_port(self_cdata->tox, &error);

  switch (error) {
    case TOX_ERR_GET_PORT_OK:
      break;
    case TOX_ERR_GET_PORT_NOT_BOUND:
      return Qnil;
    default:
      return Qnil;
  }

  const VALUE udp_port = LONG2FIX(udp_port_data);

  return udp_port;
}

// Tox::Client#tcp_port
VALUE mTox_cClient_tcp_port(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  TOX_ERR_GET_PORT error;

  const uint16_t tcp_port_data = tox_self_get_tcp_port(self_cdata->tox, &error);

  switch (error) {
    case TOX_ERR_GET_PORT_OK:
      break;
    case TOX_ERR_GET_PORT_NOT_BOUND:
      return Qnil;
    default:
      return Qnil;
  }

  const VALUE tcp_port = LONG2FIX(tcp_port_data);

  return tcp_port;
}

// Tox::Client#bootstrap
VALUE mTox_cClient_bootstrap(const VALUE self, const VALUE address, const VALUE port, const VALUE public_key)
{
  Check_Type(address, T_STRING);

  if (!rb_funcall(public_key, rb_intern("is_a?"), 1, mTox_cPublicKey)) {
    RAISE_TYPECHECK("Tox::Client#bootstrap", "public_key", "Tox::PublicKey");
  }

  StringValueCStr(address);

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const VALUE public_key_value = rb_funcall(public_key, rb_intern("value"), 0);

  const char *address_data = RSTRING_PTR(address);
  const uint16_t port_data = NUM2USHORT(port);
  const char *public_key_data = RSTRING_PTR(public_key_value);

  TOX_ERR_BOOTSTRAP error;

  const bool result = tox_bootstrap(
    self_cdata->tox,
    address_data,
    port_data,
    public_key_data,
    &error
  );

  switch (error) {
    case TOX_ERR_BOOTSTRAP_OK:
      break;
    case TOX_ERR_BOOTSTRAP_NULL:
      return Qfalse;
    case TOX_ERR_BOOTSTRAP_BAD_HOST:
      return Qfalse;
    case TOX_ERR_BOOTSTRAP_BAD_PORT:
      return Qfalse;
    default:
      return Qfalse;
  }

  if (!result) {
    return Qfalse;
  }

  return Qtrue;
}

// Tox::Client#add_tcp_relay
VALUE mTox_cClient_add_tcp_relay(const VALUE self, const VALUE address, const VALUE port, const VALUE public_key)
{
  Check_Type(address, T_STRING);

  if (!rb_funcall(public_key, rb_intern("is_a?"), 1, mTox_cPublicKey)) {
    RAISE_TYPECHECK("Tox::Client#bootstrap", "public_key", "Tox::PublicKey");
  }

  StringValueCStr(address);

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const VALUE public_key_value = rb_funcall(public_key, rb_intern("value"), 0);

  const char *address_data = RSTRING_PTR(address);
  const uint16_t port_data = NUM2USHORT(port);
  const char *public_key_data = RSTRING_PTR(public_key_value);

  TOX_ERR_BOOTSTRAP error;

  const bool result = tox_add_tcp_relay(
    self_cdata->tox,
    address_data,
    port_data,
    public_key_data,
    &error
  );

  switch (error) {
    case TOX_ERR_BOOTSTRAP_OK:
      break;
    case TOX_ERR_BOOTSTRAP_NULL:
      return Qfalse;
    case TOX_ERR_BOOTSTRAP_BAD_HOST:
      return Qfalse;
    case TOX_ERR_BOOTSTRAP_BAD_PORT:
      return Qfalse;
    default:
      return Qfalse;
  }

  if (!result) {
    return Qfalse;
  }

  return Qtrue;
}

// Tox::Client#name
VALUE mTox_cClient_name(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const size_t name_size_data = tox_self_get_name_size(self_cdata->tox);

  char name_data[name_size_data];

  if (name_size_data > 0) {
    tox_self_get_name(self_cdata->tox, name_data);
  }

  const VALUE name = rb_str_new(name_data, name_size_data);

  return name;
}

// Tox::Client#name=
VALUE mTox_cClient_name_ASSIGN(const VALUE self, const VALUE name)
{
  Check_Type(name, T_STRING);

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  TOX_ERR_SET_INFO error;

  const bool result = tox_self_set_name(
    self_cdata->tox,
    RSTRING_PTR(name),
    RSTRING_LEN(name),
    &error
  );

  switch (error) {
    case TOX_ERR_SET_INFO_OK:
      break;
    case TOX_ERR_SET_INFO_NULL:
      RAISE_FUNC_ERROR(
        "tox_self_set_name",
        mTox_eNullError,
        "TOX_ERR_SET_INFO_NULL"
      );
    case TOX_ERR_SET_INFO_TOO_LONG:
      RAISE_FUNC_ERROR(
        "tox_self_set_name",
        rb_eRuntimeError,
        "TOX_ERR_SET_INFO_TOO_LONG"
      );
    default:
      RAISE_FUNC_ERROR_DEFAULT("tox_self_set_name");
  }

  if (!result) {
    RAISE_FUNC_RESULT("tox_self_set_name");
  }

  return name;
}

// Tox::Client#status
VALUE mTox_cClient_status(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const TOX_USER_STATUS status_data = tox_self_get_status(self_cdata->tox);

  const VALUE status = mTox_mUserStatus_FROM_DATA(status_data);

  return status;
}

// Tox::Client#status=
VALUE mTox_cClient_status_ASSIGN(const VALUE self, const VALUE status)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const TOX_USER_STATUS status_data = mTox_mUserStatus_TO_DATA(status);

  tox_self_set_status(self_cdata->tox, status_data);

  return status;
}

// Tox::Client#status_message
VALUE mTox_cClient_status_message(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const size_t status_message_size_data =
    tox_self_get_status_message_size(self_cdata->tox);

  char status_message_data[status_message_size_data];

  if (status_message_size_data > 0) {
    tox_self_get_status_message(self_cdata->tox, status_message_data);
  }

  const VALUE status_message =
    rb_str_new(status_message_data, status_message_size_data);

  return status_message;
}

// Tox::Client#status_message=
VALUE mTox_cClient_status_message_ASSIGN(const VALUE self, const VALUE status_message)
{
  Check_Type(status_message, T_STRING);

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  TOX_ERR_SET_INFO error;

  const bool result = tox_self_set_status_message(
    self_cdata->tox,
    RSTRING_PTR(status_message),
    RSTRING_LEN(status_message),
    &error
  );

  switch (error) {
    case TOX_ERR_SET_INFO_OK:
      break;
    case TOX_ERR_SET_INFO_NULL:
      RAISE_FUNC_ERROR(
        "tox_self_set_status_message",
        mTox_eNullError,
        "TOX_ERR_SET_INFO_NULL"
      );
    case TOX_ERR_SET_INFO_TOO_LONG:
      RAISE_FUNC_ERROR(
        "tox_self_set_status_message",
        rb_eRuntimeError,
        "TOX_ERR_SET_INFO_TOO_LONG"
      );
    default:
      RAISE_FUNC_ERROR_DEFAULT("tox_self_set_status_message");
  }

  if (!result) {
    RAISE_FUNC_RESULT("tox_self_set_status_message");
  }

  return status_message;
}

// Tox::Client#friend_numbers
VALUE mTox_cClient_friend_numbers(const VALUE self)
{
  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const size_t friend_numbers_size = tox_self_get_friend_list_size(self_cdata->tox);

  if (friend_numbers_size == 0) {
    const VALUE friends = rb_ary_new();
    return friends;
  }

  uint32_t friend_numbers[friend_numbers_size];

  tox_self_get_friend_list(self_cdata->tox, friend_numbers);

  VALUE friend_number_values[friend_numbers_size];

  for (unsigned long i = 0; i < friend_numbers_size; ++i) {
    friend_number_values[i] = LONG2NUM(friend_numbers[i]);
  }

  const VALUE friends = rb_ary_new4(friend_numbers_size, friend_number_values);

  return friends;
}

// Tox::Client#friend_add_norequest
VALUE mTox_cClient_friend_add_norequest(const VALUE self, const VALUE public_key)
{
  if (!rb_funcall(public_key, rb_intern("is_a?"), 1, mTox_cPublicKey)) {
    RAISE_TYPECHECK("Tox::Client#friend_add_norequest", "public_key", "Tox::PublicKey");
  }

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  const VALUE public_key_value = rb_funcall(public_key, rb_intern("value"), 0);

  TOX_ERR_FRIEND_ADD error;

  const VALUE friend_number = LONG2FIX(tox_friend_add_norequest(
    self_cdata->tox,
    RSTRING_PTR(public_key_value),
    &error
  ));

  switch (error) {
    case TOX_ERR_FRIEND_ADD_OK:
      break;
    case TOX_ERR_FRIEND_ADD_NULL:
      RAISE_FUNC_ERROR(
        "tox_friend_add_norequest",
        mTox_eNullError,
        "TOX_ERR_FRIEND_ADD_NULL"
      );
    case TOX_ERR_FRIEND_ADD_OWN_KEY:
      RAISE_FUNC_ERROR(
        "tox_friend_add_norequest",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_OWN_KEY"
      );
    case TOX_ERR_FRIEND_ADD_ALREADY_SENT:
      RAISE_FUNC_ERROR(
        "tox_friend_add_norequest",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_ALREADY_SENT"
      );
    case TOX_ERR_FRIEND_ADD_BAD_CHECKSUM:
      RAISE_FUNC_ERROR(
        "tox_friend_add_norequest",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_BAD_CHECKSUM"
      );
    case TOX_ERR_FRIEND_ADD_SET_NEW_NOSPAM:
      RAISE_FUNC_ERROR(
        "tox_friend_add_norequest",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_SET_NEW_NOSPAM"
      );
    case TOX_ERR_FRIEND_ADD_MALLOC:
      RAISE_FUNC_ERROR(
        "tox_friend_add_norequest",
        rb_eNoMemError,
        "TOX_ERR_FRIEND_ADD_MALLOC"
      );
    default:
      RAISE_FUNC_ERROR_DEFAULT("tox_friend_add_norequest");
  }

  const VALUE friend = rb_funcall(self, rb_intern("friend"), 1,
                                  friend_number);

  return friend;
}

// Tox::Client#friend_add
VALUE mTox_cClient_friend_add(const VALUE self, const VALUE address, const VALUE text)
{
  if (!rb_funcall(address, rb_intern("is_a?"), 1, mTox_cAddress)) {
    RAISE_TYPECHECK("Tox::Client#friend_add", "address", "Tox::Address");
  }

  Check_Type(text, T_STRING);

  CDATA(self, mTox_cClient_CDATA, self_cdata);

  TOX_ERR_FRIEND_ADD error;

  const VALUE address_value = rb_funcall(address, rb_intern("value"), 0);

  const VALUE friend_number = LONG2FIX(tox_friend_add(
    self_cdata->tox,
    RSTRING_PTR(address_value),
    RSTRING_PTR(text),
    RSTRING_LEN(text),
    &error
  ));

  switch (error) {
    case TOX_ERR_FRIEND_ADD_OK:
      break;
    case TOX_ERR_FRIEND_ADD_NULL:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        mTox_eNullError,
        "TOX_ERR_FRIEND_ADD_NULL"
      );
    case TOX_ERR_FRIEND_ADD_NO_MESSAGE:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        mTox_eNullError,
        "TOX_ERR_FRIEND_ADD_NO_MESSAGE"
      );
    case TOX_ERR_FRIEND_ADD_TOO_LONG:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        mTox_eNullError,
        "TOX_ERR_FRIEND_ADD_TOO_LONG"
      );
    case TOX_ERR_FRIEND_ADD_OWN_KEY:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_OWN_KEY"
      );
    case TOX_ERR_FRIEND_ADD_ALREADY_SENT:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_ALREADY_SENT"
      );
    case TOX_ERR_FRIEND_ADD_BAD_CHECKSUM:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_BAD_CHECKSUM"
      );
    case TOX_ERR_FRIEND_ADD_SET_NEW_NOSPAM:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        rb_eRuntimeError,
        "TOX_ERR_FRIEND_ADD_SET_NEW_NOSPAM"
      );
    case TOX_ERR_FRIEND_ADD_MALLOC:
      RAISE_FUNC_ERROR(
        "tox_friend_add",
        rb_eNoMemError,
        "TOX_ERR_FRIEND_ADD_MALLOC"
      );
  }

  const VALUE friend = rb_funcall(self, rb_intern("friend"), 1,
                                  friend_number);

  return friend;
}

/*************************************************************
 * Private methods
 *************************************************************/

// Tox::Client#initialize_with
VALUE mTox_cClient_initialize_with(const VALUE self, const VALUE options)
{
  if (!rb_funcall(options, rb_intern("is_a?"), 1, mTox_cOptions)) {
    RAISE_TYPECHECK("Tox::Client#initialize_with", "options", "Tox::Options");
  }

  CDATA(self,    mTox_cClient_CDATA,  self_cdata);
  CDATA(options, mTox_cOptions_CDATA, options_cdata);

  TOX_ERR_NEW error;

  self_cdata->tox = tox_new(options_cdata->tox_options, &error);

  switch (error) {
    case TOX_ERR_NEW_OK:
      break;
    case TOX_ERR_NEW_NULL:
      RAISE_FUNC_ERROR(
        "tox_new",
        mTox_eNullError,
        "TOX_ERR_NEW_NULL"
      );
    case TOX_ERR_NEW_MALLOC:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eNoMemError,
        "TOX_ERR_NEW_MALLOC"
      );
    case TOX_ERR_NEW_PORT_ALLOC:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eRuntimeError,
        "TOX_ERR_NEW_PORT_ALLOC"
      );
    case TOX_ERR_NEW_PROXY_BAD_TYPE:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eRuntimeError,
        "TOX_ERR_NEW_PROXY_BAD_TYPE"
      );
    case TOX_ERR_NEW_PROXY_BAD_HOST:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eRuntimeError,
        "TOX_ERR_NEW_PROXY_BAD_HOST"
      );
    case TOX_ERR_NEW_PROXY_BAD_PORT:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eRuntimeError,
        "TOX_ERR_NEW_PROXY_BAD_PORT"
      );
    case TOX_ERR_NEW_PROXY_NOT_FOUND:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eRuntimeError,
        "TOX_ERR_NEW_PROXY_NOT_FOUND"
      );
    case TOX_ERR_NEW_LOAD_ENCRYPTED:
      RAISE_FUNC_ERROR(
        "tox_new",
        rb_eRuntimeError,
        "TOX_ERR_NEW_LOAD_ENCRYPTED"
      );
    case TOX_ERR_NEW_LOAD_BAD_FORMAT:
      RAISE_FUNC_ERROR(
        "tox_new",
        mTox_cClient_eBadSavedataError,
        "TOX_ERR_NEW_LOAD_BAD_FORMAT"
      );
    default:
      RAISE_FUNC_ERROR_DEFAULT("tox_new");
  }

  if (!self_cdata->tox) {
    RAISE_FUNC_RESULT("tox_new");
  }

  // Self callbacks
  tox_callback_self_connection_status(self_cdata->tox, on_self_connection_status_change);

  // Friend callbacks
  tox_callback_friend_request          (self_cdata->tox, on_friend_request);
  tox_callback_friend_message          (self_cdata->tox, on_friend_message);
  tox_callback_friend_name             (self_cdata->tox, on_friend_name_change);
  tox_callback_friend_status_message   (self_cdata->tox, on_friend_status_message_change);
  tox_callback_friend_status           (self_cdata->tox, on_friend_status_change);
  tox_callback_friend_connection_status(self_cdata->tox, on_friend_connection_status_change);

  // File callbacks
  tox_callback_file_chunk_request(self_cdata->tox, on_file_chunk_request);
  tox_callback_file_recv         (self_cdata->tox, on_file_recv_request);
  tox_callback_file_recv_chunk   (self_cdata->tox, on_file_recv_chunk);
  tox_callback_file_recv_control (self_cdata->tox, on_file_recv_control);

  return self;
}
