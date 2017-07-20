#!/bin/sh
curl -X POST localhost:9080/log -H "Content-Type: application/json" -d "@testLogPayload.json"
