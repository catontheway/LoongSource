--����ɱ ������� 1008073 ��10%���ʴ�����������һ���ű���
--ˢ�� �Űħ˧ 1008086 ���꣺x 2412 y 26387 z 707 ������������ ������� 1008073 ��ʧ
--ˢ�� ����ħ˧ 1008087 ���꣺x 2817 y 26387 z 708 ������������ ������� 1008073 ��ʧ
--ˢ�� ���ħ˧ 1008088 ���꣺x 2795 y 26387 z 310 ������������ ������� 1008073 ��ʧ
--ˢ�� ̰����� 1008078 ���꣺x 2436 y 26387 z 355
--ˢ�� ̰����� 1008079 ���꣺x 2377 y 26387 z 741
--ˢ�� ̰����� 1008080 ���꣺x 2840 y 26387 z 298
--��1%���ʴ�������ű���
--ˢ�� ������ɷ��ħ�� 1008081 ���꣺x 2600 y 25797 z 447 ������������ ������� 1008073 ��ʧ

function c1008073_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(1,100)
	local i = math.random(1,6)
	if k > 90 then
		if i == 1 then
			map.MapCreateCreature(MapID, InstanceID, 1008086, 2412, 26387, 707)
			--map.MapDeleteCreature(MapID, InstanceID, 1008073)
		elseif i == 2 then
			map.MapCreateCreature(MapID, InstanceID, 1008087, 2817, 26387, 708)
			--map.MapDeleteCreature(MapID, InstanceID, 1008073)
		elseif i == 3 then
			map.MapCreateCreature(MapID, InstanceID, 1008088, 2795, 26387, 310)
			--map.MapDeleteCreature(MapID, InstanceID, 1008073)
		elseif i == 4 then
			map.MapCreateCreature(MapID, InstanceID, 1008078, 2436, 26387, 355)
		elseif i == 5 then
			map.MapCreateCreature(MapID, InstanceID, 1008079, 2377, 26387, 741)
		elseif i == 6 then
			map.MapCreateCreature(MapID, InstanceID, 1008080, 2840, 26387, 298)
		end
	elseif k == 1 then
			map.MapCreateCreature(MapID, InstanceID, 1008081, 2600, 25797, 447)
			map.MapDeleteCreature(MapID, InstanceID, 1008073)
	end
end
aux.RegisterCreatureEvent(1008073, 4, "c1008073_OnDie")

-- �������ս������ʾ��
function c1008073_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008073, 2, "c1008073_OnEnterCombat")
