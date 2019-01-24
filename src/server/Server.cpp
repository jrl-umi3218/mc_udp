#include <mc_nng/server/Server.h>

#include <mc_nng/logging.h>

namespace mc_nng
{

Server::Server(const std::string & uri, int timeout)
{
  start(uri, timeout);
}

bool Server::recv()
{
  uint8_t * buffer = nullptr;
  size_t size = 0;
  int err = nng_recv(socket_, &buffer, &size, NNG_FLAG_ALLOC);
  if(err == 0)
  {
    do
    {
      err = nng_recv(socket_, &buffer, &size, NNG_FLAG_ALLOC | NNG_FLAG_NONBLOCK);
      if(err == 0)
      {
        MC_NNG_INFO("nng_recv got more data from the socket")
      }
    } while(err == 0);
    err = 0;
    control_.fromBuffer(buffer);
    nng_free(buffer, size);
  }
  else
  {
    if(err != NNG_ETIMEDOUT)
    {
      MC_NNG_WARNING("nng_recv failed: " << nng_strerror(err))
    }
    nng_free(buffer, size);
  }
  return err == 0;
}

void Server::send()
{
  auto size = sensors_.size();
  auto buffer = nng_alloc(size);
  sensors_.toBuffer(reinterpret_cast<uint8_t*>(buffer));
  int err = nng_send(socket_, buffer, size, NNG_FLAG_NONBLOCK | NNG_FLAG_ALLOC);
  if(err != 0)
  {
    if(err != NNG_ETIMEDOUT)
    {
      MC_NNG_WARNING("nng_send failed: " << nng_strerror(err))
    }
    nng_free(buffer, size);
  }
}

void Server::stop()
{
  nng_close(socket_);
}

void Server::restart(const std::string & uri, int timeout)
{
  stop();
  start(uri, timeout);
}

void Server::start(const std::string & uri, int timeout)
{
  int err = nng_pair0_open(&socket_);
  if(err != 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to create socket: " << nng_strerror(err))
  }
  err = nng_listen(socket_, uri.c_str(), nullptr, 0);
  if(err != 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to create listener: " << nng_strerror(err))
  }
  err = nng_setopt_ms(socket_, NNG_OPT_RECVTIMEO, timeout);
  if(err != 0)
  {
    MC_NNG_ERROR_AND_THROW(std::runtime_error, "Failed to set recv timeout: " << nng_strerror(err))
  }
}

}
