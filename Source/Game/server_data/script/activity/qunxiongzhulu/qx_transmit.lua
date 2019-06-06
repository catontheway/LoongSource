--[[������ȼ��õ�����ID
local function RoleLevel2InstanceID(RoleLevel)
	if RoleLevel == nil then
		return 0
	end

	if RoleLevel < 40 then
		return 0
	elseif RoleLevel < 60 then
		return 1
	elseif RoleLevel < 80 then
		return 2
	elseif RoleLevel < 100 then
		return 3
	else
		return 4
	end
end

----�ж���Ҷ����Ƿ�Ϸ�
local function TestRoleTeam(MapID, InstanceID, RoleID, TeamRoleData, InstanceIndex)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])
	if TeamID ~= 4294967295 then
	    --���С�����ID
	    local Role = {}
	    Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	    for i = 1, 6 do]]
--			if Role[i]~=4294967295 and (not TeamRoleData[TeamKey][Role[i]]) then
--[[			return false
			end
		end

		--�жϱ���ʱ��С�ӳ�Ա�Ƿ���ͬһ����
	    for k in pairs(TeamRoleData[TeamKey]) do
		    local ID = role.IsRoleHaveTeam(2983743695, InstanceIndex, k)
		    if ID and ID ~= TeamID then
			    return false
		    end
	    end

	    return true
	else
		return true
	end
end


--�ж�����Ƿ�Ϊ�������뿪�
local function IsDeadLeave(MapID, InstanceID, RoleID, RoleLevel)
	local RoleExpire = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_Expire"])
	local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])

	--if RoleLevel < 60 then
		if RoleExpire ~= act.GetActScriptData(18, 1, 0) then
			return false, 0
		elseif Act18_TeamRoleData[TeamKey] and Act18_TeamRoleData[TeamKey][RoleID] then
			return true, 2000
		end
	--elseif RoleLevel < 80 then
		if RoleExpire ~= act.GetActScriptData(19, 1, 0) then
			return false, 0
		elseif Act19_TeamRoleData[TeamKey] and Act19_TeamRoleData[TeamKey][RoleID] then
			return true, 5000
		end
	--elseif RoleLevel < 100 then
		if RoleExpire ~= act.GetActScriptData(20, 1, 0) then
			return false, 0
		elseif Act20_TeamRoleData[TeamKey] and Act20_TeamRoleData[TeamKey][RoleID] then
			return true, 10000
		end
	--else
		if RoleExpire ~= act.GetActScriptData(21, 1, 0) then
			return false, 0
		elseif Act21_TeamRoleData[TeamKey] and Act21_TeamRoleData[TeamKey][RoleID] then
			return true, 20000
		else
		    return false, 0
		end
	--end
end

--��ջ����
function ClearAct18Data(TeamRoleData, TeamData)
	local Empty = {}
	TeamRoleData = Empty
	TeamData = Empty
end

--�������Ƿ�μӹ��
function TestRoleExpire(MapID, InstanceID, RoleID)
    local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    local InstanceIndex = RoleLevel2InstanceID(RoleLevel)
    local RoleExpire = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_Expire"])

    if InstanceIndex == 1 then
        if RoleExpire == Act18_Expire then
            return true
        end
    end

    if InstanceIndex == 2 then
        if RoleExpire == Act19_Expire then
            return true
        end
    end

    if InstanceIndex == 3 then
        if RoleExpire == Act20_Expire then
            return true
        end
    end

    if InstanceIndex == 4 then
        if RoleExpire == Act21_Expire then
            return true
        end
    end

    return false
end

--NPC���ͽ��볡��todo
function C4500201_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	--�жϻ�Ƿ��Ѿ���ʼ
	local bStart = (act.GetActIsStart(18) or act.GetActIsStart(103))

	if -1 == TalkIndex then
	    if PVPactivity_trigger == 1 then
			--��Ѿ���ʼ
			if bStart then
				local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				if RoleLevel < 40 then
					--��ʾ��ҵȼ��������޷�����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	157)
					msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					return
				end

				--���������������뿪����ʾ����ٴν�����Ҫ���ѽ�Ǯ
				local bDeadLeave, Money = IsDeadLeave(MapID, InstanceID, RoleID, RoleLevel)
				if bDeadLeave then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 151)
					msg.AddMsgEvent(MsgID, 9, Money)
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	4)			--ȷ��
					msg.AddMsgEvent(MsgID, 1,	21)
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif TestRoleExpire(MapID, InstanceID, RoleID) then  --�������ǵ��ߺ��뿪�Ļ
					--��ʾ����Ƿ��������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	160)		--���Ƿ�Ҫ����Ӣ��������
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	4)			--ȷ��
					msg.AddMsgEvent(MsgID, 1,	21)
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					--��ʾ���û�б������޷�����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	162)		--Ⱥ����¹�����ʱ�仹û�п�ʼ
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else --���û�п�ʼ
				--����Ƿ�Ϊ��ı���ʱ��
				local Hour = tonumber(os.date("%H"))
				local Min  = tonumber(os.date("%M"))
				local Week = tonumber(os.date("%w"))
				if (Week == 0 or Week == 3) and Hour == 19 and Min >=50 then
					--�ж�����Ƿ��Ѿ�������
					if TestRoleExpire(MapID, InstanceID, RoleID) then  --�������ǵ��ߺ��뿪�Ļ
						--��ʾ����Ƿ��������
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20,	160)		--���Ƿ�Ҫ����Ӣ��������
						msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
						msg.AddMsgEvent(MsgID, 21,	4)			--ȷ��
						msg.AddMsgEvent(MsgID, 1,	21)
						msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
						msg.AddMsgEvent(MsgID, 1,	22)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						return
					end

					--��һ�û�б���
					local Success = true
					local LeaderID = 4294967295

					--�жϱ������Ƿ��Ƕӳ�
					local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
					if TeamID == -1 then
						Success = false
					end

					if Success then
						LeaderID = team.GetTeamLeaderID(TeamID)
						if LeaderID ~= RoleID then
							Success = false
						end
					end

					--�ж�С�ӳ�Ա�����͵ȼ�
					if Success then
						--���С�����ID
						local Role = {}
						Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
						local InstanceIndex = RoleLevel2InstanceID(role.GetRoleLevel(MapID, InstanceID, RoleID))
						for k in pairs(Role) do
							if 4294967295 ~= Role[k] then
								local RoleLevel = role.GetRoleLevel(MapID, InstanceID, Role[k])
								if InstanceIndex ~= RoleLevel2InstanceID(RoleLevel) then
									Success = false
									break
								end
							else
								Success = false
								break
							end
						end
					end

					if Success then
						--��ʾ����Ƿ��������
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20,	160)		--���Ƿ�Ҫ����Ӣ��������
						msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
						msg.AddMsgEvent(MsgID, 21,	4)			--ȷ��
						msg.AddMsgEvent(MsgID, 1,	21)
						msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
						msg.AddMsgEvent(MsgID, 1,	22)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)

					else
						--��ʾ�����ʾ�����������ϱ����������μ�Ⱥ����¹���������ӷ�ʽ�������Ҷ����������6�ˣ������˱���Ϊ�ӳ�����Ա�ȼ�������ͬһ����ڡ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20,	161)
						msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
						msg.AddMsgEvent(MsgID, 1,	22)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					--��ʾ���û�е������ʱ��
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	162)		--Ⱥ����¹�����ʱ�仹û�п�ʼ
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		else
		    --��ʾ��һû�п�ʼ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2863)			--Ⱥ����¹��ݲ����ţ������½⡣
			msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
			msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end

	--������ѡ��ȷ��
    if TalkIndex == 4 then
	    local InstanceIndex = RoleLevel2InstanceID(role.GetRoleLevel(MapID, InstanceID, RoleID))
	    --��Ѿ���ʼ
        if bStart then
	        local RoleExpire = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_Expire"])
	        --������
	        if InstanceIndex == 1 then
	            --�õ�����ϴβμӻʱ�Ľ���, ����͵�ǰ���������ȣ���������ҽ���
                if RoleExpire ~= act.GetActScriptData(18, 1, 0) then
	                return
	            else --�ж�������ڶ����Ƿ��Ǳ���ʱ�Ķ���
					local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act18_TeamRoleData, InstanceIndex)
					if false == bRet then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2864)		--Ⱥ����¹�����ʱ���ѹ�
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						return
					end
			    end
	        end

	        --������
	        if InstanceIndex == 2 then
                if RoleExpire ~= act.GetActScriptData(19, 1, 0) then
	                return
	            else --�ж�������ڶ����Ƿ��Ǳ���ʱ�Ķ���
	                local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act19_TeamRoleData, InstanceIndex)
					if false == bRet then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2864)		--Ⱥ����¹�����ʱ���ѹ�
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						return
					end
				end
	        end

	        --Ӣ����
	        if InstanceIndex == 3 then
                if RoleExpire ~= act.GetActScriptData(20, 1, 0) then
	                return
	            else --�ж�������ڶ����Ƿ��Ǳ���ʱ�Ķ���
	                local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act20_TeamRoleData, InstanceIndex)
					if false == bRet then
					    local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2864)		--Ⱥ����¹�����ʱ���ѹ�
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						return
					end
	            end
	        end

	        --������
	        if InstanceIndex == 4 then
                if RoleExpire ~= act.GetActScriptData(21, 1, 0) then
	                return
	            else --�ж�������ڶ����Ƿ��Ǳ���ʱ�Ķ���
	                local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act21_TeamRoleData, InstanceIndex)
					if false == bRet then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2864)		--Ⱥ����¹�����ʱ���ѹ�
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						return
					end
	            end
	        end

			--������ҵ������
			local Index = math.random(1, 8)
			role.RoleGotoNewMap(MapID, InstanceIndex, RoleID, 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
        else    --�û�п�ʼ
			--�õ�����ϴβμӻʱ�Ľ���, ����͵�ǰ���������ȣ�������С�ӳ�Ա�ĵȼ�������
			local RoleExpire = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_Expire"])
	        if InstanceIndex == 1 then
	            --���û�д��������
                if IsCreateAct18Instance == 0 then
	                --��ʼ������������
				    ClearAct18Data(Act18_TeamRoleData, Act18_TeamData)
	                Act18_TeamIndex = 1

                    Act18_Expire = act.GetActScriptData(18, 1, 0)
	                if Act18_Expire == 0 then
					    --����ǵ�һ������ʼ����ǰ����Ϊ10001��������ȡ����ʱ���жϣ�
					    Act18_Expire = 10000
	                end

	                --��ʼ�����ǰ����
	                Act18_Expire = Act18_Expire + 1
	                act.SetActScriptData(18, 1, 0, Act18_Expire)
	                act.SaveActScriptData(18)

                    --���������
	                map.CreateInstance(2983743695, 1)
                    IsCreateAct18Instance = 1
	            end

                if RoleExpire ~= Act18_Expire then
					--�ж�С�ӳ�Ա�����͵ȼ�
		            local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
                    local Success = true

                    --���С�����ID
		            local Role = {}
		            Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
        		    for k in pairs(Role) do
	    	    	    if Role[k] and 4294967295 ~= Role[k] then
							local RoleLevel = role.GetRoleLevel(MapID, InstanceID, Role[k])
			    	        if InstanceIndex ~= RoleLevel2InstanceID(RoleLevel) then
				    	        Success = false
					            break
			    	        end
			            else
				            Success = false
				            break
			            end
		            end

                    if Success then
--                         table.insert(Act18_TeamRoleData, Act18_TeamIndex, {})
                        --������ҵ������
						local Index = math.random(1, 8)
		                for i=1, 6 do
		                    role.SetRoleScriptData(Role[i], 1, RoleDataType["Act18_TeamKey"], Act18_TeamIndex)
		                    role.RoleGotoNewMap(MapID, InstanceID, Role[i], 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
		                end

		                --�С�ӱ�ż�һ
                        Act18_TeamIndex = Act18_TeamIndex + 1
                    end
	            else	--����Ѿ�����
					local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act18_TeamRoleData, 1)
					if bRet then
						--������ҵ������
						local Index = math.random(1, 8)
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
					end
	            end
	        end

	        if InstanceIndex == 2 then
	            --���û�д��������
                if IsCreateAct19Instance == 0 then
					--��ʼ������������
	                ClearAct18Data(Act19_TeamRoleData, Act19_TeamData)
	                Act19_TeamIndex = 1

                    Act19_Expire = act.GetActScriptData(19, 1, 0)
	                if Act19_Expire == 0 then
	                    --����ǵ�һ������ʼ����ǰ����Ϊ20001��������ȡ����ʱ���жϣ�
	                    Act19_Expire = 20001
	                end

	                --��ʼ�����ǰ����
	                Act19_Expire = Act19_Expire + 1
	                act.SetActScriptData(19, 1, 0, Act19_Expire)
	                act.SaveActScriptData(19)

                    --���������
	                map.CreateInstance(2983743695, 2)
                    IsCreateAct19Instance = 1
	            end

                if RoleExpire ~= Act19_Expire then
	                --�ж�С�ӳ�Ա�����͵ȼ�
		            local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
                    local Success = true

                    --���С�����ID
		            local Role = {}
		            Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					for k in pairs(Role) do
	    	    	    if Role[k] and 4294967295 ~= Role[k] then
							local RoleLevel = role.GetRoleLevel(MapID, InstanceID, Role[k])
			    	        if InstanceIndex ~= RoleLevel2InstanceID(RoleLevel) then
				        	    Success = false
					            break
			      	        end
						else
				            Success = false
				            break
			            end
		            end

                    if Success then
--                        table.insert(Act19_TeamRoleData, Act19_TeamIndex, {})
--                        Act19_TeamRoleData[Act19_TeamIndex] = {}

             			--������ҵ������
						local Index = math.random(1, 8)
						for i=1, 6 do
						    role.SetRoleScriptData(Role[i], 1, RoleDataType["Act18_TeamKey"], Act19_TeamIndex)
		                    role.RoleGotoNewMap(MapID, InstanceID, Role[i], 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
		                end

		                --�С�ӱ�ż�һ
                        Act19_TeamIndex = Act19_TeamIndex + 1
                    end
	            else  --����Ѿ�����
					local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act19_TeamRoleData, 2)
					if bRet then
						--������ҵ������
						local Index = math.random(1, 8)
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
					end
	            end
	        end

	        if InstanceIndex == 3 then
	            --���û�д��������
                if IsCreateAct20Instance == 0 then
					--��ʼ������������
	                ClearAct18Data(Act20_TeamRoleData, Act20_TeamData)
	                Act20_TeamIndex = 1
                    Act20_Expire = act.GetActScriptData(20, 1, 0)

	                if Act20_Expire == 0 then
	                    --����ǵ�һ������ʼ����ǰ����Ϊ30001��������ȡ����ʱ���жϣ�
	                     Act20_Expire = 30001
	                end

	                --��ʼ�����ǰ����
	                Act20_Expire = Act20_Expire + 1
	                act.SetActScriptData(20, 1, 0, Act20_Expire)
	                act.SaveActScriptData(20)

                    --���������
	                map.CreateInstance(2983743695, 3)
                    IsCreateAct20Instance = 1
	            end

                if RoleExpire ~= Act20_Expire then
	     	        --�ж�С�ӳ�Ա�����͵ȼ�
		            local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
                    local Success = true

                    --���С�����ID
		            local Role = {}
		            Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					for k in pairs(Role) do
	    	    	    if Role[k] and 4294967295 ~= Role[k] then
							local RoleLevel = role.GetRoleLevel(MapID, InstanceID, Role[k])
			    	        if InstanceIndex ~= RoleLevel2InstanceID(RoleLevel) then
				    	        Success = false
					            break
			    	        end
			            else
				            Success = false
				            break
			            end
		            end

                    if Success then
--                        table.insert(Act20_TeamRoleData, Act20_TeamIndex, {})
--                        Act20_TeamRoleData[Act20_TeamIndex] = {}

                        --������ҵ������
		                local Index = math.random(1, 8)
		                for i=1, 6 do
		                    role.SetRoleScriptData(Role[i], 1, RoleDataType["Act18_TeamKey"], Act20_TeamIndex)
		                    role.RoleGotoNewMap(MapID, InstanceID, Role[i], 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
		                end

		                --�С�ӱ�ż�һ
                        Act20_TeamIndex = Act20_TeamIndex + 1
                    end
                else   --����Ѿ�����
		            local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act20_TeamRoleData, 3)
					if bRet then
						--������ҵ������
						local Index = math.random(1, 8)
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
					end
                end
	        end

	        if InstanceIndex == 4 then
	            --���û�д��������
                if IsCreateAct21Instance == 0 then
					--��ʼ������������
	                ClearAct18Data(Act21_TeamRoleData, Act21_TeamData)
	                Act21_TeamIndex = 1

                    Act21_Expire = act.GetActScriptData(21, 1, 0)
					if Act21_Expire == 1 then
						--����ǵ�һ������ʼ����ǰ����Ϊ40001��������ȡ����ʱ���жϣ�
	                    Act21_Expire = 40001
	                end

	                --��ʼ�����ǰ����
	                Act21_Expire = Act21_Expire + 1
	                act.SetActScriptData(21, 1, 0, Act21_Expire)
	                act.SaveActScriptData(21)

                    --���������
	                map.CreateInstance(2983743695, 4)
                    IsCreateAct21Instance = 1
	            end

                if RoleExpire ~= Act21_Expire then
					--�ж�С�ӳ�Ա�����͵ȼ�
		            local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
                    local Success = true

                    --���С�����ID
		            local Role = {}
		            Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
					for k in pairs(Role) do
	    	    	    if Role[k] and 4294967295 ~= Role[k] then
							local RoleLevel = role.GetRoleLevel(MapID, InstanceID, Role[k])
			    	        if InstanceIndex ~= RoleLevel2InstanceID(RoleLevel) then
				    	        Success = false
					            break
			    	        end
			            else
							Success = false
				            break
			            end
		            end

                    if Success then
--                        table.insert(Act21_TeamRoleData, Act21_TeamIndex, {})
--                        Act21_TeamRoleData[Act21_TeamIndex] = {}

                        --������ҵ������
		                local Index = math.random(1, 8)
		                for i=1, 6 do
		                    role.SetRoleScriptData(Role[i], 1, RoleDataType["Act18_TeamKey"], Act21_TeamIndex)
		                    role.RoleGotoNewMap(MapID, InstanceID, Role[i], 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
		                end

		                --�С�ӱ�ż�һ
                        Act21_TeamIndex = Act21_TeamIndex + 1
                    end
	            else --����Ѿ�����
					local bRet = TestRoleTeam(MapID, InstanceID, RoleID, Act21_TeamRoleData, 4)
					if bRet then
						--������ҵ������
						local Index = math.random(1, 8)
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2983743695, m22_BornPos[Index].x, m22_BornPos[Index].y, m22_BornPos[Index].z)
					end
	            end
            end
        end
    end
end

--ע����ҽ����¼�
aux.RegisterCreatureEvent(4500201, 7, "C4500201_OnTalk")]]
