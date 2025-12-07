#!/bin/bash

# === Ask user for server selection ===
echo "Select target server:"
echo "  1) Server 01 (pizero01.local)"
echo "  2) Server 02 (pizero02.local)"
read -r -p "Enter 1 or 2: " choice

case "$choice" in
    1)
        TARGET_HOST="pizero01.local"
        ;;
    2)
        TARGET_HOST="pizero02.local"
        ;;
    *)
        echo "Invalid choice. Exiting."
        exit 1
        ;;
esac

echo ""
echo "You selected: $TARGET_HOST"
echo "⚠️  WARNING: This action will OVERWRITE source code on the server at $REMOTE_PATH"
read -r -p "Are you sure? (Y/N): " confirm

# Confirm intent
if [[ "$confirm" != "Y" && "$confirm" != "y" ]]; then
    echo "Aborted."
    exit 0
fi

echo ""
echo "Ensuring remote directory exists..."
ssh "${REMOTE_USER}@${TARGET_HOST}" "mkdir -p $REMOTE_PATH"

echo "Copying ./src/ → ${REMOTE_USER}@${TARGET_HOST}:$REMOTE_PATH"
scp -r ./src/* "${REMOTE_USER}@${TARGET_HOST}:$REMOTE_PATH"

if [[ $? -eq 0 ]]; then
    echo "✅ Deployment complete."
else
    echo "❌ Deployment failed."
fi
