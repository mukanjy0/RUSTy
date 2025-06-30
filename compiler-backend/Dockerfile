FROM python:3.11-slim

RUN apt-get update && apt-get install -y --fix-missing g++ && rm -rf /var/lib/apt/lists/*
WORKDIR /app

COPY compiler-backend/backend.py .
COPY compiler-backend/requirements.txt .
COPY src/ ./src/
COPY main.cpp .

RUN pip install --no-cache-dir -r requirements.txt

EXPOSE 8000

CMD ["uvicorn", "backend:app", "--host", "0.0.0.0", "--port", "8000", "--reload"]