FROM python:3.11-slim-bullseye as compile-image
RUN python -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
COPY requirements.txt .
RUN pip install --no-cache-dir --upgrade pip \
 && pip install --no-cache-dir -r requirements.txt \
 && pip install --no-cache-dir "psycopg[binary,pool]"

WORKDIR /app
COPY app /app
CMD ["python", "-m", "bot"]

# docker build -t neechan9/cloud-computing-task:latest .
# docker image tag neechan9/cloud-computing-task:latest neechan9/cloud-computing-task:latest
# docker image push neechan9/cloud-computing-task:latest