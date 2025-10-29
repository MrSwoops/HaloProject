#pragma once

#include "CoreMinimal.h"

/**
 * GameplayEvents namespace replacement for Unity-style event system.
 */
namespace GlobalEventManager
{
// Subscription flags
enum class ESubscriptionFlags : uint8
{
    None            = 0,
    TriggerOnce     = 1 << 0,
    ActiveActorOnly = 1 << 1
};
ENUM_CLASS_FLAGS(ESubscriptionFlags)

// Forward declare
class FSubscriberEntryBase;

// Base class for all subscriber entries
class FSubscriberEntryBase
{
public:
    virtual ~FSubscriberEntryBase() = default;
};

// Template for a specific message type
template <typename MessageType>
class TSubscriberEntry : public FSubscriberEntryBase
{
public:
    struct FSubscription
    {
        TWeakObjectPtr<UObject> Subscriber;
        TFunction<void(const MessageType&)> Callback;
        ESubscriptionFlags Flags;
    };

    TArray<FSubscription> Subscriptions;

    void Add(UObject* Subscriber, TFunction<void(const MessageType&)> Callback, ESubscriptionFlags Flags)
    {
        FSubscription Sub;
        Sub.Subscriber = Subscriber;
        Sub.Callback = Callback;
        Sub.Flags = Flags;
        Subscriptions.Add(Sub);
    }

    void Raise(const MessageType& Message)
    {
        for (int32 i = 0; i < Subscriptions.Num();)
        {
            auto& Sub = Subscriptions[i];

            UObject* Subscriber = Sub.Subscriber.Get();
            if (!Subscriber)
            {
                Subscriptions.RemoveAt(i);
                continue;
            }

            // Handle ActiveActorOnly flag
            if (EnumHasAnyFlags(Sub.Flags, ESubscriptionFlags::ActiveActorOnly))
            {
                AActor* Actor = Cast<AActor>(Subscriber);
                if (Actor && !Actor->IsActorTickEnabled())
                {
                    ++i;
                    continue;
                }
            }

            // Call subscriber function
            Sub.Callback(Message);

            // Handle TriggerOnce
            if (EnumHasAnyFlags(Sub.Flags, ESubscriptionFlags::TriggerOnce))
            {
                Subscriptions.RemoveAt(i);
            }
            else
            {
                ++i;
            }
        }
    }
};

class FGlobalEventManager
{
private:
    static TMap<FName, TUniquePtr<FSubscriberEntryBase>>& GetEvents()
    {
        static TMap<FName, TUniquePtr<FSubscriberEntryBase>> Events;
        return Events;
    }

    template <typename T>
    static FName GetMessageKey()
    {
        return FName(typeid(T).name());
    }

public:
    template <typename SubscriberType, typename MessageType>
    static void Subscribe(
        SubscriberType* Subscriber,
        TFunction<void(const MessageType&)> OnMessageCalled,
        bool bOnlyTriggerIfActive = false,
        bool bTriggerOnce = false)
    {
        static_assert(TIsDerivedFrom<SubscriberType, UObject>::IsDerived, "SubscriberType must derive from UObject");

        ESubscriptionFlags Flags = ESubscriptionFlags::None;
        if (bOnlyTriggerIfActive)
            Flags |= ESubscriptionFlags::ActiveActorOnly;
        if (bTriggerOnce)
            Flags |= ESubscriptionFlags::TriggerOnce;

        FName Key = GetMessageKey<MessageType>();

        auto& Events = GetEvents();

        if (!Events.Contains(Key))
        {
            Events.Add(Key, MakeUnique<TSubscriberEntry<MessageType>>());
        }

        TSubscriberEntry<MessageType>* Entry = static_cast<TSubscriberEntry<MessageType>*>(Events[Key].Get());
        Entry->Add(Subscriber, OnMessageCalled, Flags);
    }
    template <typename MessageType>
    static void RaiseEvent(const MessageType& Message)
    {
        FName Key = GetMessageKey<MessageType>();
        auto& Events = GetEvents();

        if (TUniquePtr<FSubscriberEntryBase>* Found = Events.Find(Key))
        {
            if (auto* TypedEntry = static_cast<TSubscriberEntry<MessageType>*>(Found->Get()))
            {
                TypedEntry->Raise(Message);
            }
        }
    }

    static void ClearAll()
    {
        GetEvents().Empty();
    }
};
}



/*

Subscribe Example
GameplayEvents::FGlobalEventManager::Subscribe<AMyCharacter, FPlayerKilledMessage>(
    this,
    [this](const FPlayerKilledMessage& Msg)
    {
        UE_LOG(LogTemp, Log, TEXT("%s heard that %s killed %s"), *GetName(), *Msg.KillerName, *Msg.VictimName);
    },
    false,  // onlyTriggerIfActive
    false   // triggerOnce
);

Call Example
FPlayerKilledMessage Msg;
Msg.KillerName = TEXT("PlayerA");
Msg.VictimName = TEXT("PlayerB");

GameplayEvents::FGlobalEventManager::RaiseEvent(Msg);
 
 */