// Copyright (c) 2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SIDECHAIN_H
#define BITCOIN_SIDECHAIN_H

#include <primitives/transaction.h>
#include <pubkey.h>

#include <array>

// These are the temporary values to speed things up during testing
static const int SIDECHAIN_VERIFICATION_PERIOD = 32;
static const int SIDECHAIN_MIN_WORKSCORE = 16;

// These are the values that will be used in the final release
//static const int SIDECHAIN_VERIFICATION_PERIOD = 26298;
//static const int SIDECHAIN_MIN_WORKSCORE = 13140;

//! Sidechain deposit fee (TODO make configurable per sidechain)
static const CAmount SIDECHAIN_DEPOSIT_FEE = 0.00001 * COIN;

//! Max number of failures (blocks without commits) for a sidechain to activate
static const int SIDECHAIN_ACTIVATION_MAX_FAILURES = 32;
//! The amount of time a sidechain has to activate
static const int SIDECHAIN_ACTIVATION_MAX_AGE = 64;
//! The number of sidechains which may be signaled for activation at once
static const int SIDECHAIN_ACTIVATION_MAX_SIGNALS = 32;
//! The number of sidechains which may be active at once
static const int SIDECHAIN_ACTIVATION_MAX_ACTIVE = 256;

//! The current sidechain version
static const int SIDECHAIN_VERSION_CURRENT = 0;
//! The max supported sidechain version
static const int SIDECHAIN_VERSION_MAX = 0;

struct SidechainProposal {
    int32_t nVersion = SIDECHAIN_VERSION_CURRENT;
    std::string title;
    std::string description;
    std::string sidechainKeyID;
    std::string sidechainHex;
    std::string sidechainPriv;
    uint256 hashID1;
    uint160 hashID2;

    bool DeserializeFromScript(const CScript& script);

    std::vector<unsigned char> GetBytes() const;
    CScript GetScript() const;
    uint256 GetHash() const;
    bool operator==(const SidechainProposal& proposal) const;
    std::string ToString() const;

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nVersion);
        READWRITE(title);
        READWRITE(description);
        READWRITE(sidechainKeyID);
        READWRITE(sidechainHex);
        READWRITE(sidechainPriv);
        READWRITE(hashID1);
        READWRITE(hashID2);
    }
};

struct SidechainActivationStatus
{
    int nAge;
    int nFail;
    SidechainProposal proposal;

    uint256 GetHash() const;

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nAge);
        READWRITE(nFail);
        READWRITE(proposal);
    }
};

struct Sidechain {
    int32_t nVersion = SIDECHAIN_VERSION_CURRENT;
    uint8_t nSidechain;
    std::string sidechainKeyID;
    std::string sidechainPriv;
    std::string sidechainHex;
    std::string title;
    std::string description;
    uint256 hashID1;
    uint160 hashID2;

    std::string GetSidechainName() const;
    bool operator==(const Sidechain& a) const;
    bool operator==(const SidechainProposal& a) const;
    std::string ToString() const;
    uint256 GetHash() const;

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nVersion);
        READWRITE(nSidechain);
        READWRITE(sidechainKeyID);
        READWRITE(sidechainPriv);
        READWRITE(sidechainHex);
        READWRITE(title);
        READWRITE(description);
        READWRITE(hashID1);
        READWRITE(hashID2);
    }
};

struct SidechainDeposit {
    uint8_t nSidechain;
    CKeyID keyID;
    CMutableTransaction tx;
    uint32_t n;
    uint256 hashBlock;

    bool operator==(const SidechainDeposit& a) const;
    std::string ToString() const;
    uint256 GetHash() const;

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nSidechain);
        READWRITE(keyID);
        READWRITE(tx);
        READWRITE(n);
        READWRITE(hashBlock);
    }
};

static const char SCDB_UPVOTE = 'u';
static const char SCDB_DOWNVOTE = 'd';
static const char SCDB_ABSTAIN = 'a';

// A vote set by the user to specify custom votes for particular WT^(s) - Used
// by the miner to pass minimal data GenerateSCDBUpdateScript()
struct SidechainCustomVote
{
    char vote; // SCDB_UPVOTE, SCDB_DOWNVOTE or SCDB_ABSTAIN
    uint8_t nSidechain;
    uint256 hashWTPrime;

    bool operator==(const SidechainCustomVote& v) const
    {
        return (vote == v.vote && nSidechain == v.nSidechain
                && hashWTPrime == v.hashWTPrime);
    }

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(vote);
        READWRITE(nSidechain);
        READWRITE(hashWTPrime);
    }
};

struct SidechainWTPrimeState {
    uint8_t nSidechain;
    uint16_t nBlocksLeft;
    uint16_t nWorkScore;
    uint256 hashWTPrime;

    bool IsNull() const;
    uint256 GetHash() const;
    bool operator==(const SidechainWTPrimeState& a) const;
    std::string ToString() const;

    // For hash calculation
    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nSidechain);
        READWRITE(nBlocksLeft);
        READWRITE(nWorkScore);
        READWRITE(hashWTPrime);
    }
};

struct SidechainSpentWTPrime {
    uint8_t nSidechain;
    uint256 hashWTPrime;
    uint256 hashBlock;

    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nSidechain);
        READWRITE(hashWTPrime);
        READWRITE(hashBlock);
    }
};

struct SidechainCTIP {
    COutPoint out;
    CAmount amount;

    uint256 GetHash() const;

    // For hash calculation
    ADD_SERIALIZE_METHODS

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(out);
        READWRITE(amount);
    }
};

#endif // BITCOIN_SIDECHAIN_H
