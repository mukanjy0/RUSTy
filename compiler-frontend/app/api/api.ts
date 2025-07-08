import axios from "axios";

const BACKEND_URL =
    process.env.NEXT_PUBLIC_API_URL || "http://localhost:8000";

export const postCode = async ({ code, signal }: { code: string; signal?: AbortSignal }) => {
  console.log("Fetching Code to", BACKEND_URL);
  const response = await axios.post(`${BACKEND_URL}/compile`, { code }, { signal });
  return response.data;
};

export const runTests = async (signal?: AbortSignal) => {
  const response = await axios.get(`${BACKEND_URL}/run-tests`, { signal });
  return response.data;
};