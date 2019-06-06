--[[�̶��Ӣ������

--����������(����RoleID, RebornTime, Score��IsDeadLeave, IsInMap, JoinTime)
Act14_RoleData = {} --������
Act15_RoleData = {} --������
Act16_RoleData = {} --Ӣ����
Act17_RoleData = {} --������

--�̶���ѿ�ʼ��ʱ�䣨��λ�Ƿ��ӣ�
act14_LastTime = 0



--NPC���ͽ��볡��todo
function C4500101_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	--�жϻ�Ƿ��Ѿ���ʼ
	local bStart = act.GetActIsStart(14)

	if -1 == TalkIndex then
		--��Ѿ���ʼ
		if bStart then
			--��ʾ����Ƿ��������

			--���������������뿪����ʾ����ٴν�����Ҫ���ѽ�Ǯ

		else --���û�п�ʼ
			--��ʾ��һû�п�ʼtodo
		end
	end

	--������ѡ��ȷ��
	if TalkIndex == 4 then
		local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if RoleLevel < 40 then
			--��ʾ��ҵȼ��������޷�����
		else if RoleLevel < 60 then
			--������ҵ�������

			--���������������뿪���۳���ҽ�Ǯ


		else if RoleLevel < 80 then
			--������ҵ�������

			--���������������뿪���۳���ҽ�Ǯ

		else if RoleLevel < 100 then
			--������ҵ�Ӣ����

			--���������������뿪���۳���ҽ�Ǯ

		else
			--������ҵ�������

			--���������������뿪���۳���ҽ�Ǯ

		end


	end
end

function

--ע����ҽ����¼�
aux.RegisterCreatureEvent(4500101, 7, "C4500101_OnTalk")

--�̶����ʼ
function act14_OnStart(ActID)
	--��ʼ����ѽ���ʱ��
	act14_LastTime = 0

	--��ʼ������������
	Act14_RoleData = nil
	Act15_RoleData = nil
	Act16_RoleData = nil
	Act17_RoleData = nil
end

function act14_OnTimerMin(ActID)
	--�̶���ѿ�ʼ+1
	act14_LastTime = act14_LastTime + 1

	--���ʼ��ĵ�5������ʱ�䣨��5��10����50��55���ӣ�, ������һ���+5
	if act14_LastTime % 5 == 0 then
		--������
		for k in pairs(Act14_RoleData) do
			if nil == Act14_RoleData[k] then
				break
			end

			--����ڻ��ͼ��
			if Act14_RoleData[k].IsInMap then
				--���û������
				if unit.IsDead(XXXXX, -1, k) then
					Act14_RoleData[k].Score = Act14_RoleData[k].Score + 5
				end
			end
		end

		--������
		for n in pairs(Act15_RoleData) do
			if nil == Act15_RoleData[n] then
				break
			end

			--����ڻ��ͼ��
			if Act15RoleData[n].IsInMap then
				--���û������
				if unit.IsDead(XXXXX, -1, n) then
					Act15_RoleData[n].Score = Act15_RoleData[n].Score + 5
				end
			end
		end

		--Ӣ����
		for m in pairs(Act16_RoleData) do
			if nil == Act16_RoleData[m] then
				break
			end

			--����ڻ��ͼ��
			if Act16_RoleData[m].IsInMap then
				--���û������
				if unit.IsDead(XXXXX, -1, m) then
					Act16_RoleData[m].Score = Act16_RoleData[m].Score + 5
				end
			end
		end

		----������
		for i in pairs(Act17_RoleData) do
			if nil == Act17_RoleData[i] then
				break
			end

			--����ڻ��ͼ��
			if Act17_RoleData[i].IsInMap then
				--���û������
				if unit.IsDead(XXXXX, -1, i) then
					Act17_RoleData[i].Score = Act17_RoleData[i].Score + 5
				end
			end
		end

		--���ˢ��5�ֲ�ͬ���͵ľ���
		for j=1, 5 do
			local CrystalBallID = math.random(1520001, 1520019)
			--���һ������todo

			--ˢ��ˮ����

		end
	end

	--�ڻ��ʼ��ÿ��15���ӣ����ڻ������ˢ�³�1�����ǹ���
	if act14_LastTime % 15 == 0 then
		--������
		if ActID = 15 then
			map.MapCreateColCreature(MapID, InstanceID, 1520020, XXXX, XXXX, XXXX, 0, "")
		end

		--Ӣ����
		if ActID = 16 then
			map.MapCreateColCreature(MapID, InstanceID, 1520021, XXXX, XXXX, XXXX, 0, "")
		end

		--������
		if ActID = 17 then
			map.MapCreateColCreature(MapID, InstanceID, 1520022, XXXX, XXXX, XXXX, 0, "")
		end
	end

	--ÿ��10���ӣ��Զ���ѡ������ڵ�3����ң������ϡ�������״̬
	if act14_LastTime % 10 == 0 then
		if ActID = 16 then
			local SelectedNum	= 0   		--�Ѿ�ѡ�е�������������������
			local CircleNum		= 20		--��������
			local RoleNum		= table.getn(Act16_RoleData)
			while CircleNum == 0 do
				local Index = math.random(1, RoleNum)
				for k in pairs(Act16_RoleData) do
					Index = Index - 1
					if Index == 0 then
						if Act16_RoleData[k].IsInMap == true and false == unit.IsDead(k) then
							--�������ӡ�������״̬ todo
							SelectedNum = SelectedNum + 1
							break
						end
					end
				end

				CircleNum = CircleNum - 1
				if SelectedNum == 3 then
					break;
				end
			end
		end

		if ActID = 17 then
			local SelectedNum	= 0   		--�Ѿ�ѡ�е�������������������
			local CircleNum		= 20		--��������
			local RoleNum		= table.getn(Act17_RoleData)
			while CircleNum == 0 do
				local Index = math.random(1, RoleNum)
				for k in pairs(Act17_RoleData) do
					Index = Index - 1
					if Index == 0 then
						if Act17_RoleData[k].IsInMap == true and false == unit.IsDead(k) then
							--�������ӡ�������״̬ todo
							SelectedNum = SelectedNum + 1
							break
						end
					end
				end

				CircleNum = CircleNum - 1
				if SelectedNum == 3 then
					break;
				end
			end
		end
	end

	--������һ�������
end

--�����
function act14_OnEnd(actID)

end

--ע��û������¼�
aux.RegisterActEvent(14, 2, "act14_OnStart") 	--������
aux.RegisterActEvent(15, 2, "act14_OnStart") 	--������
aux.RegisterActEvent(16, 2, "act14_OnStart") 	--Ӣ����
aux.RegisterActEvent(17, 2, "act14_Onstart") 	--������
aux.RegisterActEvent(14, 4, "act14_OnTimerMin") --������
aux.RegisterActEvent(15, 4, "act14_OnTimerMin") --������
aux.RegisterActEvent(16, 4, "act14_OnTimerMin") --Ӣ����
aux.RegisterActEvent(17, 4, "act14_OnTimerMin") --������
aux.RegisterActEvent(14, 3, "act14_OnEnd") 		--������
aux.RegisterActEvent(14, 3, "act14_OnEnd")		--������
aux.RegisterActEvent(14, 3, "act14_OnEnd")		--Ӣ����
aux.RegisterActEvent(14, 3, "act14_OnEnd")		--������

--------------------------------------------------------------------------------------------------------
--todo��ҽ�������
function m21_PlayerEnter(MapID, InstanceID, RoleID)
	--��������һ��15����޵�buff
	unit.AddBuff(MapID, InstanceID, RoleID,

	--����Ƿ��ǵ�һ�ν���
	if Act14_RoleData[RoleID] then
		--��һ�ν��룬����û��RoleData��
		local RoleDataTemp = {RebornTime=0, Score=0, IsDeadLeave=false, IsInMap=true, JoinTime=0}
		table.insert(Act14_RoleData, RoleID, RoleDataTemp)
	else
		--���������1
		Act14_RoleData[RoleID].RebornTime = Act14_RoleData[RoleID].RebornTime + 1
		Act14_RoleData[RoleID].IsInMap = true
	end
end

function m22_PlayerEnter(MapID, InstanceID, RoleID)
	--��������һ��15����޵�buff, todo
	unit.AddBuff(MapID, InstanceID, RoleID,

	--����Ƿ��ǵ�һ�ν���
	if Act15_RoleData[RoleID] then
		--��һ�ν��룬����û��RoleData��
		local RoleDataTemp = {RebornTime=0, Score=0, IsDeadLeave=false, IsInMap=true, JoinTime=0}
		table.insert(Act15_RoleData, RoleID, RoleDataTemp)
	else
		--���������1
		Act15_RoleData[RoleID].RebornTime = Act15_RoleData[RoleID].RebornTime + 1
		Act15_RoleData[RoleID].IsInMap = true
	end
end

function m23_PlayerEnter(MapID, InstanceID, RoleID)
	--��������һ��15����޵�buff, todo
	unit.AddBuff(MapID, InstanceID, RoleID,

	--����Ƿ��ǵ�һ�ν���
	if Act16_RoleData[RoleID] then
		--��һ�ν��룬����û��RoleData��
		local RoleDataTemp = {RebornTime=0, Score=0, IsDeadLeave=false, IsInMap=true, JoinTime=0}
		table.insert(Act16_RoleData, RoleID, RoleDataTemp)
	else
		--���������1
		Act16_RoleData[RoleID].RebornTime = Act16_RoleData[RoleID].RebornTime + 1
		Act16_RoleData[RoleID].IsInMap = true
	end
end

function m24_PlayerEnter(MapID, InstanceID, RoleID)
	--��������һ��15����޵�buff, todo
	unit.AddBuff(MapID, InstanceID, RoleID,

	--����Ƿ��ǵ�һ�ν���
	if Act17_RoleData[RoleID] then
		--��һ�ν��룬����û��RoleData��
		local RoleDataTemp = {RebornTime=0, Score=0, IsDeadLeave=false, IsInMap=true, JoinTime=0}
		table.insert(Act17_RoleData, RoleID, RoleDataTemp)
	else
		--���������1
		Act17_RoleData[RoleID].RebornTime = Act17_RoleData[RoleID].RebornTime + 1
		Act17_RoleData[RoleID].IsInMap = true
	end
end

--ע�������ͼ�¼�
aux.RegisterMapEvent(2983744463, 2, "m21_PlayerEnter")
aux.RegisterMapEvent(2983743695, 2, "m22_PlayerEnter")
aux.RegisterMapEvent(2983743951, 2, "m23_PlayerEnter")
aux.RegisterMapEvent(2983745231, 2, "m24_PlayerEnter")


-------------------------------------------------------------------------------------------------------
--����뿪�����
function m21_PlayerEnter(MapID, InstanceID, RoleID)
	Act14_RoleData[RoleID].IsInMap = false
end

function m22_PlayerEnter(MapID, InstanceID, RoleID)
	Act15_RoleData[RoleID].IsInMap = false
end

function m23_PlayerEnter(MapID, InstanceID, RoleID)
	Act16_RoleData[RoleID].IsInMap = false
end

function m24_PlayerEnter(MapID, InstanceID, RoleID)
	Act17_RoleData[RoleID].IsInMap = false
end

--ע���뿪���ͼ�¼�
aux.RegisterMapEvent(2983744463, 3, "m21_PlayerEnter")
aux.RegisterMapEvent(2983743695, 3, "m22_PlayerEnter")
aux.RegisterMapEvent(2983743951, 3, "m23_PlayerEnter")
aux.RegisterMapEvent(2983745231, 3, "m24_PlayerEnter")

-------------------------------------------------------------------------------------------------------
--����ڻ�е��ߺ�����
function m21_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo

	return 1, dwOutMapID, x, y, z
end

aux.RegisterMapEvent(2983744463,  12, "m21_CanTakeOverWhenOnline")
aux.RegisterMapEvent(2983743695,  12, "m21_CanTakeOverWhenOnline")
aux.RegisterMapEvent(2983743951,  12, "m21_CanTakeOverWhenOnline")
aux.RegisterMapEvent(2983745231,  12, "m21_CanTakeOverWhenOnline")


-------------------------------------------------------------------------------------------------------
--����������todo
function m21_RoleDie(MapID, InstanceID, RoleID, KillerID)

end

function m22_RoleDie(MapID, InstanceID, RoleID, KillerID)

end

function m23_RoleDie(MapID, InstanceID, RoleID, KillerID)

end

function m24_RoleDie(MapID, InstanceID, RoleID, KillerID)

end

aux.RegisterMapEvent(2983744463, 5, "m21_RoleDie")
aux.RegisterMapEvent(2983743695, 5, "m22_RoleDie")
aux.RegisterMapEvent(2983743951, 5, "m23_RoleDie")
aux.RegisterMapEvent(2983745231, 5, "m24_RoleDie")]]





















