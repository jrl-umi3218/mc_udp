#include <mc_nng/client/Client.h>

#include <mc_nng/logging.h>

namespace mc_nng
{

Client::Client(const std::string & uri, int timeout)
{
  int err = nng_pair0_open(&socket_);
  if(err != 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to create socket: " << nng_strerror(err))
  }
  err = nng_dial(socket_, uri.c_str(), nullptr, 0);
  if(err != 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to create dialer: " << nng_strerror(err))
  }
  err = nng_setopt_ms(socket_, NNG_OPT_RECVTIMEO, timeout);
  if(err != 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to set recv timeout: " << nng_strerror(err))
  }
}

bool Client::recv()
{
  uint8_t * buffer = nullptr;
  size_t size = 0;
  int err = nng_recv(socket_, &buffer, &size, NNG_FLAG_ALLOC);
  if(err == 0)
  {
    sensors_.fromBuffer(buffer);
    nng_free(buffer, size);
  }
  else
  {
    if(err != NNG_ETIMEDOUT)
    {
      MC_NNG_WARNING("nng_recv failed: " << nng_strerror(err))
    }
  }
  return err == 0;
}

void Client::send()
{
  auto size = control_.size();
  auto buffer = nng_alloc(size);
  control_.toBuffer(reinterpret_cast<uint8_t*>(buffer));
  int err = nng_send(socket_, buffer, size, NNG_FLAG_ALLOC);
  if(err != 0)
  {
    MC_NNG_WARNING("nng_send failed: " << nng_strerror(err))
    nng_free(buffer, size);
  }
}

}
