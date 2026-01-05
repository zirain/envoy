#pragma once

#include <chrono>
#include <ratio>

#include "envoy/ratelimit/ratelimit.h"

namespace Envoy {
namespace Extensions {
namespace Filters {
namespace Common {
namespace LocalRateLimit {

using XRateLimitHeadersRFCVersion =
    envoy::extensions::common::ratelimit::v3::XRateLimitHeadersRFCVersion;

class TokenBucketContext {
public:
  virtual ~TokenBucketContext() = default;

  virtual uint64_t maxTokens() const PURE;
  virtual uint64_t remainingTokens() const PURE;
  virtual uint64_t resetSeconds() const PURE;
};

// Interface for a local rate limiter.
class LocalRateLimiter {
public:
  struct Result {
    bool allowed{};
    XRateLimitHeadersRFCVersion rate_limit_headers_rfc_version{XRateLimitHeadersRFCVersion::OFF};
    std::shared_ptr<const TokenBucketContext> token_bucket_context{};
  };

  virtual ~LocalRateLimiter() = default;

  // Returns true if the request should be rate limited.
  virtual Result requestAllowed(absl::Span<const RateLimit::Descriptor> request_descriptors) = 0;
};
using LocalRateLimiterSharedPtr = std::shared_ptr<LocalRateLimiter>;

} // namespace LocalRateLimit
} // namespace Common
} // namespace Filters
} // namespace Extensions
} // namespace Envoy
